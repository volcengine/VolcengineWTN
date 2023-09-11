package whipsdk

import (
	"context"
	"errors"
	"io"
	"net/http"
	"testing"

	"github.com/bytedance/sonic"
	"github.com/jarcoal/httpmock"
	"github.com/stretchr/testify/assert"
)

func fakeGetter(rtcAppId string, vCode VendorCode) *VendorInfo {
	return &VendorInfo{
		VendorCode: vCode,
		VAppId:     "123",
		Domain:     "http://www.domain.com",
		Secret:     "secret",
	}

}

func TestWhipReq_IsValid(t *testing.T) {
	var req *WhipReq
	assert.False(t, req.IsValid())

	req = &WhipReq{Action: actionSub}
	assert.False(t, req.IsValid())

	req = &WhipReq{Action: "Unknown"}
	assert.False(t, req.IsValid())

	req = &WhipReq{
		VendorCode:        1,
		AppId:             "appid",
		SDP:               "sdp",
		ClientIP:          "",
		Location:          "",
		Action:            actionPub,
		StreamId:          "streamid",
		SessionId:         "SessionId",
		DisableDownlinkFb: false,
		QueryParams:       nil,
		HeaderParams:      nil,
	}

	assert.True(t, req.IsValid())

	req.Action = actionUpdate
	assert.False(t, req.IsValid())
	req.Action = actionDelete
	assert.False(t, req.IsValid())
	req.Location = "location"
	req.Action = actionUpdate
	assert.True(t, req.IsValid())
	req.Action = actionDelete
	assert.True(t, req.IsValid())

	req.SDP = ""
	req.Action = actionDelete
	assert.True(t, req.IsValid())
}

func TestWhipProxy_SendWhip(t *testing.T) {
	fakeLocation := `https://a.b.c`
	ctx := context.Background()

	wReq := &WhipReq{
		VendorCode:        CodeAgora,
		AppId:             "rtcAppid",
		SDP:               "i am sdp offer!",
		ClientIP:          "1.1.1.1",
		Location:          "",
		Action:            actionPub,
		StreamId:          "streamId",
		SessionId:         "ec009f99f21abf21dfd1b3cbxbq4g89t",
		DisableDownlinkFb: true,
	}

	t.Logf("pub req:%s\n", MarshalToString(wReq))

	proxy := NewWhipProxy(WithSecret([]byte("secret")), WithVendorInfoGetter(fakeGetter))

	// mock
	httpmock.ActivateNonDefault(proxy.(*whipProxy).client)
	defer httpmock.DeactivateAndReset()

	httpmock.RegisterResponder("POST", "=~.*/(push|pub)/.*",
		func(req *http.Request) (*http.Response, error) {
			return nil, errors.New("server err")
		},
	)
	resp, err := proxy.SendWhip(ctx, wReq)
	assert.Nil(t, resp)
	code, _, origin := GetCodeAndMessage(err)
	assert.Equal(t, http.StatusInternalServerError, code)
	assert.Equal(t, MsgOriginWhipserver, origin)

	httpmock.RegisterResponder("POST", "=~.*/(push|pub)/.*",
		func(req *http.Request) (*http.Response, error) {
			t.Logf("%+v", req)
			assert.Equal(t, 32, len(req.URL.Query().Get("SessionID")))
			assert.Equal(t, wReq.SessionId[:24], req.URL.Query().Get("SessionID")[:24])

			body, _ := io.ReadAll(req.Body)
			assert.Equal(t, "i am sdp offer!", string(body))
			resp := httpmock.NewBytesResponse(200, []byte("i am SDP answer"))
			resp.Header.Set(Location, fakeLocation)
			return resp, nil
		},
	)

	resp, err = proxy.SendWhip(ctx, wReq)
	assert.Nil(t, err)
	assert.Equal(t, http.StatusOK, resp.StatusCode)
	assert.Equal(t, "i am SDP answer", resp.Body)
	assert.Equal(t, fakeLocation, resp.Headers[Location][0])

	httpmock.RegisterResponder("POST", "=~.*/(pull|sub)/.*",
		func(req *http.Request) (*http.Response, error) {
			t.Logf("%+v", req)
			assert.Equal(t, "true", req.URL.Query().Get("DisableDownlinkFb"))
			resp := httpmock.NewBytesResponse(200, []byte("i am pull SDP answer"))
			resp.Header.Set(Location, fakeLocation)
			return resp, nil
		},
	)
	wReq.Action = actionSub
	resp, err = proxy.SendWhip(ctx, wReq)
	assert.Nil(t, err)
	assert.Equal(t, http.StatusOK, resp.StatusCode)
	assert.Equal(t, "i am pull SDP answer", resp.Body)
	assert.Equal(t, fakeLocation, resp.Headers[Location][0])

	httpmock.RegisterResponder("POST", "=~.*/(pull|sub)/.*",
		func(req *http.Request) (*http.Response, error) {
			t.Logf("%+v", req)
			resp := httpmock.NewBytesResponse(409, []byte("stream not exist!"))
			return resp, nil
		},
	)
	wReq.Action = actionSub
	resp, err = proxy.SendWhip(ctx, wReq)
	assert.Nil(t, err)
	assert.Equal(t, 409, resp.StatusCode)
	assert.Equal(t, "stream not exist!", resp.Body)
}

func TestWhipProxy_updateAndDel(t *testing.T) {
	fakeLocation := `https://a.b.c/sub/appid/streamid`
	ctx := context.Background()
	muteAudio := false
	wReq := &WhipReq{
		VendorCode: CodeTrtc,
		AppId:      "456",
		SDP:        "sdp",
		MuteAudio:  &muteAudio,
		ClientIP:   "1.1.1.1",
		Location:   "",
		Action:     actionUpdate,
		StreamId:   "streamid",
		SessionId:  "sid",
	}

	proxy := NewWhipProxy(WithSecret([]byte("secret")), WithVendorInfoGetter(fakeGetter))

	// mock
	httpmock.ActivateNonDefault(proxy.(*whipProxy).client)
	defer httpmock.DeactivateAndReset()

	httpmock.RegisterResponder("POST", "=~.*",
		func(req *http.Request) (*http.Response, error) {
			body, _ := io.ReadAll(req.Body)
			t.Logf("%s", body)
			assert.Equal(t, req.URL.String(), fakeLocation)
			resp := httpmock.NewBytesResponse(200, []byte("ok"))

			mute := map[string]bool{}
			sonic.Unmarshal(body, &mute)
			muteA, ok := mute["MuteAudio"]
			assert.Equal(t, false, muteA)
			assert.Equal(t, true, ok)

			muteV, ok := mute["MuteVideo"]
			assert.Equal(t, false, muteV)
			assert.Equal(t, false, ok)

			return resp, nil
		},
	)
	resp, err := proxy.SendWhip(ctx, wReq)
	assert.Equal(t, CodeBadRequest, err.(*WhipError).Code)

	wReq.Location = fakeLocation
	resp, err = proxy.SendWhip(ctx, wReq)
	assert.Nil(t, err)
	assert.Equal(t, 200, resp.StatusCode)
	assert.Equal(t, "ok", resp.Body)

	httpmock.RegisterResponder("DELETE", "=~.*",
		func(req *http.Request) (*http.Response, error) {
			body, _ := io.ReadAll(req.Body)
			t.Logf("%s", body)
			assert.Equal(t, req.URL.String(), fakeLocation)
			resp := httpmock.NewBytesResponse(200, []byte("ok"))
			return resp, nil
		},
	)
	wReq.Location = fakeLocation
	wReq.Action = actionDelete
	resp, err = proxy.SendWhip(ctx, wReq)
	assert.Nil(t, err)
	assert.Equal(t, 200, resp.StatusCode)
	assert.Equal(t, "ok", resp.Body)
}

func TestWhipProxy_CheckToken(t *testing.T) {
	secret := []byte("secret")
	proxy := NewWhipProxy(WithVendorInfoGetter(fakeGetter))

	ctx := context.Background()

	params := `{
    "vendorCode":2,
    "appId":"rtcAppid",
    "sdp":"i am sdp offer!",
    "muteAudio":false,
    "muteVideo":false,
    "clientIP":"1.1.1.1",
    "location":"",
    "action":"pub",
    "streamId":"streamId",
    "sessionId":"ec009f99f21abf21dfd1b3cbxbq4g89t",
    "disableDownlinkFb":true,
    "queryParams":null,
    "headerParams":null
}`
	token := GenByteRTCToken(params, secret)
	t.Logf("token:%s\n", token)

	_, err := proxy.CheckToken(ctx, params, "xxx")
	assert.Equal(t, CodeTokenInvalid, err.(*WhipError).Code)
	assert.Equal(t, ErrTokenIllegal, err.(*WhipError).Msg)

	_, err = proxy.CheckToken(ctx, params, "unkonwnType xxx")
	assert.Equal(t, CodeTokenInvalid, err.(*WhipError).Code)
	assert.Equal(t, ErrTokenIllegal, err.(*WhipError).Msg)

	_, err = proxy.CheckToken(ctx, params, token)
	assert.Equal(t, CodeTokenInvalid, err.(*WhipError).Code)
	assert.Equal(t, ErrTokenCheckFailed, err.(*WhipError).Msg)

	_, err = proxy.CheckToken(ctx, params, "")
	assert.Equal(t, CodeTokenInvalid, err.(*WhipError).Code)
	assert.Equal(t, ErrTokenMissing, err.(*WhipError).Msg)

	proxy.UpdateSecret(secret)

	_, err = proxy.CheckToken(ctx, "checkfailed", token)
	assert.Equal(t, CodeTokenInvalid, err.(*WhipError).Code)
	assert.Equal(t, ErrTokenCheckFailed, err.(*WhipError).Msg)

	req, err := proxy.CheckToken(ctx, params, token)
	assert.Nil(t, err)
	reqStruct := WhipReq{}
	sonic.Unmarshal([]byte(params), &reqStruct)
	assert.Equal(t, *req, reqStruct)
}

func TestNewWhipProxy(t *testing.T) {
	p := NewWhipProxy(WithVendorInfoGetter(fakeGetter)).(*whipProxy)
	ctx := context.Background()

	req := WhipReq{
		SDP: "sdp",
		HeaderParams: map[string]string{
			"Content-Type":  "application/sdp",
			"Authorization": "Bearer xxxx",
		},
		url: "https://example.whip.com",
	}
	_, err := p.addStream(ctx, &req)
	t.Logf("err:%s\n", err)
	code, _, _ := GetCodeAndMessage(err)
	assert.Equal(t, 500, code)
}
