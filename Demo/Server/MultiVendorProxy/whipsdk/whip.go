package whipsdk

import (
	"bytes"
	"context"
	"crypto/sha256"
	"crypto/tls"
	"encoding/base64"
	"fmt"
	"io"
	"net"
	"net/http"
	"net/http/httptrace"
	"net/url"
	"strings"
	"time"

	"proxy_server/whipsdk/logger"

	"github.com/bytedance/sonic"
	"github.com/golang-jwt/jwt"
)

const (
	claimsKeyParams string = "paramsHash"

	// headers
	ContentType   = "Content-Type"
	Authorization = "Authorization"
	Location      = "Location"
	TypeSDP       = "application/sdp"
	TypeJson      = "application/json"
	TypeText      = "text/plain"

	reqTimeout = time.Second * 2
)

type action = string

const (
	actionPub    action = "pub"
	actionSub    action = "sub"
	actionDelete action = "delete"
	actionUpdate action = "update"
)

const (
	ctxKeyAction = "action"
)

type WhipProxy interface {
	CheckToken(ctx context.Context, params string, token string) (req *WhipReq, err error)
	SendWhip(ctx context.Context, req *WhipReq) (resp *WhipResp, err error)
	UpdateSecret(secret []byte)
}

type whipProxy struct {
	opts   options
	client *http.Client
}

type WhipReq struct {
	VendorCode        VendorCode `json:"vendorCode"`
	AppId             string     `json:"appId"`
	SDP               string     `json:"sdp"`
	MuteAudio         *bool      `json:"muteAudio,omitempty"`
	MuteVideo         *bool      `json:"muteVideo,omitempty"`
	ClientIP          string     `json:"clientIP"`
	Location          string     `json:"location"` //更新、停止推、拉流 时使用
	Action            string     `json:"action"`   //请求动作枚举值：pub、pull、delete、update
	StreamId          string     `json:"streamId"`
	SessionId         string     `json:"sessionId"`
	DisableDownlinkFb bool       `json:"disableDownlinkFb"`

	// 透传扩展信息
	QueryParams  map[string]string `json:"queryParams"`  //存放于Query中
	HeaderParams map[string]string `json:"headerParams"` //存放于Header中

	url string
}

func (r *WhipReq) IsValid() bool {
	if r == nil {
		return false
	}

	if !InStringSlice([]string{
		actionDelete,
		actionUpdate,
		actionPub,
		actionSub,
	}, r.Action) {
		return false
	}

	if r.AppId == "" || r.VendorCode <= 0 || r.StreamId == "" {
		return false
	}

	if InStringSlice([]string{actionDelete, actionUpdate}, r.Action) && r.Location == "" {
		return false
	}

	if InStringSlice([]string{actionPub, actionSub}, r.Action) && r.SDP == "" {
		return false
	}

	return true
}

type WhipResp struct {
	StatusCode int                 `json:"statusCode"`
	Body       string              `json:"body"`
	Headers    map[string][]string `json:"headers"`
}

// 全局初始化一次即可
func NewWhipProxy(opts ...Option) WhipProxy {
	proxy := whipProxy{
		opts: options{
			logger: logger.DefaultLogger(),
		},
	}

	for _, o := range opts {
		o(&proxy.opts)
	}

	if proxy.opts.vendorInfoGetter == nil {
		panic("please set vendorInfoGetter with option whipsdk.WithVendorInfoGetter()")
	}

	dialer := net.Dialer{
		Timeout:   30 * time.Second,
		KeepAlive: 30 * time.Second,
	}

	proxyFunc := http.ProxyFromEnvironment
	if proxy.opts.proxyURL != "" {
		_u, err := url.Parse(proxy.opts.proxyURL)
		if err == nil {
			proxyFunc = http.ProxyURL(_u)
		}
	}

	proxy.client = &http.Client{
		Transport: &http.Transport{
			Proxy:                 proxyFunc,
			DialContext:           dialer.DialContext,
			ForceAttemptHTTP2:     true,
			MaxIdleConns:          100,
			IdleConnTimeout:       time.Minute * 10,
			TLSHandshakeTimeout:   10 * time.Second,
			ExpectContinueTimeout: 1 * time.Second,
		},

		CheckRedirect: func(req *http.Request, via []*http.Request) error {
			req.Header.Set(Authorization, via[0].Header.Get(Authorization))
			return nil
		},
	}

	return &proxy
}

func (wp *whipProxy) SendWhip(ctx context.Context, req *WhipReq) (resp *WhipResp, err error) {
	defer func() {
		wp.opts.logger.CtxInfof(ctx, "SendWhip: req:%+v, resp:%+v, err:%+v", req, resp, err)
	}()

	errBadReq := &WhipError{
		Code:      CodeBadRequest,
		Msg:       ErrBadRequest,
		MsgOrigin: MsgOriginWhipsdk,
	}

	if !req.IsValid() {
		return nil, errBadReq
	}

	if wp.opts.vendorInfoGetter == nil {
		errBadReq.Msg = "please set vendorInfoGetter!"
		return nil, errBadReq
	}

	vInfo := wp.opts.vendorInfoGetter(req.AppId, req.VendorCode)
	if vInfo == nil {
		return nil, &WhipError{
			Code:      http.StatusInternalServerError,
			Msg:       "not found vendor conf",
			MsgOrigin: MsgOriginWhipsdk,
		}
	}

	if err = newVendor(vInfo).GenWhipParams(ctx, req); err != nil {
		return nil, err
	}

	ctx = context.WithValue(ctx, ctxKeyAction, req.Action)

	switch req.Action {
	case actionPub:
		return wp.pub(ctx, req)
	case actionSub:
		return wp.sub(ctx, req)
	case actionUpdate:
		return wp.update(ctx, req)
	case actionDelete:
		return wp.delete(ctx, req)
	}

	errBadReq.Msg = "unknown action"
	return nil, errBadReq
}

func (wp *whipProxy) UpdateSecret(secret []byte) {
	wp.opts.secret = secret
}

func (wp *whipProxy) CheckToken(ctx context.Context, params string, token string) (req *WhipReq, err error) {
	if params == "" {
		err = &WhipError{
			Code:      CodeBadRequest,
			Msg:       ErrBadRequest,
			MsgOrigin: MsgOriginWhipsdk,
		}
		return
	}

	paramsBytes := []byte(params)
	if errmsg := wp.checkToken(ctx, paramsBytes, token); errmsg != "" {
		wp.opts.logger.CtxErrorf(ctx, "checkToken err:%v", errmsg)
		return nil, &WhipError{
			Code:      CodeTokenInvalid,
			Msg:       errmsg,
			MsgOrigin: MsgOriginWhipsdk,
		}
	}

	if err = sonic.Unmarshal(paramsBytes, &req); err != nil {
		wp.opts.logger.CtxErrorf(ctx, "params is invalid err:%s, params:%s", err, params)
		return nil, &WhipError{
			Code:      CodeBadRequest,
			Msg:       "params is format is not correct",
			MsgOrigin: MsgOriginWhipsdk,
		}
	}

	return req, nil
}

func (wp *whipProxy) checkToken(ctx context.Context, params []byte, token string) (errmsg string) {
	if token == "" {
		return ErrTokenMissing
	}

	tokenFragment := strings.Split(token, " ")
	if len(tokenFragment) != 2 || tokenFragment[0] != tokenTypeBearer {
		return ErrTokenIllegal
	}

	claimsStruct := make(jwt.MapClaims)
	tk, err := jwt.ParseWithClaims(tokenFragment[1], claimsStruct, func(t *jwt.Token) (interface{}, error) {
		return wp.opts.secret, nil
	})

	if err != nil {
		wp.opts.logger.CtxErrorf(ctx, "CheckToken err:%v", err)
		if strings.Contains(err.Error(), "expired") {
			return ErrTokenExpired
		}

		return ErrTokenCheckFailed
	}

	if tk == nil {
		wp.opts.logger.CtxErrorf(ctx, "CheckToken err, can not parse token")
		return ErrTokenCheckFailed
	}

	v, ok := claimsStruct[claimsKeyParams]
	if !ok {
		return ErrTokenMissing
	}

	pHash, _ := v.(string)
	h, err := base64.StdEncoding.DecodeString(pHash)
	if err != nil {
		return ErrTokenIllegal
	}

	sha := sha256.New()
	sha.Write(params)
	if !bytes.Equal(sha.Sum(nil), h) {
		return ErrTokenCheckFailed
	}

	return
}

func (wp *whipProxy) pub(ctx context.Context, wReq *WhipReq) (wResp *WhipResp, err error) {
	return wp.addStream(ctx, wReq)
}

func (wp *whipProxy) sub(ctx context.Context, wReq *WhipReq) (wResp *WhipResp, err error) {
	return wp.addStream(ctx, wReq)
}

func (wp *whipProxy) update(ctx context.Context, wReq *WhipReq) (wResp *WhipResp, err error) {
	mute := make(map[string]bool)
	if wReq.MuteAudio != nil {
		mute["MuteAudio"] = *wReq.MuteAudio
	}

	if wReq.MuteVideo != nil {
		mute["MuteVideo"] = *wReq.MuteVideo
	}

	rawData, _ := sonic.Marshal(mute)

	ctx, cancel := context.WithTimeout(ctx, reqTimeout)
	defer cancel()
	req, err := http.NewRequestWithContext(ctx, http.MethodPost, wReq.Location, bytes.NewReader(rawData))
	if err != nil {
		return nil, &WhipError{
			Code:      http.StatusInternalServerError,
			Msg:       "failed to generate mute request",
			MsgOrigin: MsgOriginWhipserver,
		}
	}
	req.Header.Set(ContentType, TypeJson)

	return wp.do(ctx, req)
}

func (wp *whipProxy) delete(ctx context.Context, wReq *WhipReq) (wResp *WhipResp, err error) {
	ctx, cancel := context.WithTimeout(ctx, reqTimeout)
	defer cancel()
	req, err := http.NewRequestWithContext(ctx, http.MethodDelete, wReq.Location, http.NoBody)
	if err != nil {
		return nil, &WhipError{
			Code:      CodeBadRequest,
			Msg:       "failed to generate WHIP Delete request",
			MsgOrigin: MsgOriginWhipserver,
		}
	}
	req.Header.Set(ContentType, TypeText)

	return wp.do(ctx, req)
}

func (wp *whipProxy) addStream(ctx context.Context, wReq *WhipReq) (wResp *WhipResp, err error) {
	ctx, cancel := context.WithTimeout(ctx, reqTimeout)
	defer cancel()

	reqUrl := wReq.url + "?"
	for k, v := range wReq.QueryParams {
		reqUrl += k + "=" + v + "&"
	}
	reqUrl = reqUrl[:len(reqUrl)-1]
	req, err := http.NewRequestWithContext(ctx, http.MethodPost, reqUrl, bytes.NewBufferString(wReq.SDP))
	if err != nil {
		return nil, &WhipError{
			Code:      CodeBadRequest,
			Msg:       "failed to generate WHIP request",
			MsgOrigin: MsgOriginWhipsdk,
		}
	}
	for k, v := range wReq.HeaderParams {
		req.Header.Set(k, v)
	}

	return wp.do(ctx, req)
}

func (wp *whipProxy) do(ctx context.Context, req *http.Request) (wResp *WhipResp, err error) {
	now := time.Now()
	stages := [5]*stageInfo{
		{StageName: "dns"},
		{StageName: "connect"},
		{StageName: "tlsHandshake"},
		{start: now, Start: now.String(), StageName: "wroteRequest"},
		{start: now, Start: now.String(), StageName: "GotFirstResponseByte"},
	}
	defer func() {
		traceInfo, _ := sonic.MarshalString(stages)
		wp.opts.logger.CtxInfof(ctx, "httptrace info:%s", traceInfo)
	}()

	req = req.WithContext(httptrace.WithClientTrace(req.Context(), getClientTrace(stages)))
	resp, err := wp.client.Do(req)
	if err != nil {
		wp.opts.logger.CtxErrorf(ctx, "wp.client.Do err:%s", err)
		return nil, &WhipError{
			Code:      http.StatusInternalServerError,
			Msg:       fmt.Sprintf("failed to do request for %v", ctx.Value(ctxKeyAction)),
			MsgOrigin: MsgOriginWhipserver,
		}
	}
	defer resp.Body.Close()

	rawData, _ := io.ReadAll(resp.Body)
	wResp = &WhipResp{
		StatusCode: resp.StatusCode,
		Body:       string(rawData),
		Headers: map[string][]string{
			RespHeaderMsgOrigin: []string{string(MsgOriginWhipserver)},
		},
	}

	for k, v := range resp.Header {
		wResp.Headers[k] = v
	}

	return wResp, nil
}

type stageInfo struct {
	start     time.Time
	Start     string
	Cost      string //阶段耗时
	StageName string
}

func getClientTrace(stages [5]*stageInfo) *httptrace.ClientTrace {
	trace := httptrace.ClientTrace{
		DNSStart: func(info httptrace.DNSStartInfo) {
			stages[0].start = time.Now()
			stages[0].Start = stages[0].start.String()
		},
		DNSDone: func(info httptrace.DNSDoneInfo) {
			stages[0].Cost = time.Since(stages[0].start).String()
		},
		ConnectStart: func(network, addr string) {
			stages[1].start = time.Now()
			stages[1].Start = stages[1].start.String()
		},
		ConnectDone: func(network, addr string, err error) {
			stages[1].Cost = time.Since(stages[1].start).String()
		},
		TLSHandshakeStart: func() {
			stages[2].start = time.Now()
			stages[2].Start = stages[2].start.String()
		},
		TLSHandshakeDone: func(state tls.ConnectionState, err error) {
			stages[2].Cost = time.Since(stages[2].start).String()
		},
		WroteRequest: func(info httptrace.WroteRequestInfo) {
			stages[3].Cost = time.Since(stages[3].start).String()
		},
		GotFirstResponseByte: func() {
			stages[4].Cost = time.Since(stages[4].start).String()
		},
	}

	return &trace
}
