package whipsdk

import (
	"context"
	"fmt"
	"net/http"
	"strconv"
	"time"

	"github.com/golang-jwt/jwt"
	"github.com/tencentyun/tls-sig-api-v2-golang/tencentyun"
)

// vendorCode
type VendorCode int

const (
	CodeA VendorCode = 2
	CodeT VendorCode = 3
	CodeH VendorCode = 5
)

type MediaVendor interface {
	GenWhipParams(ctx context.Context, info *WhipReq) error
}

func newVendor(vInfo *VendorInfo) MediaVendor {
	switch vInfo.VendorCode {
	case CodeA:
		return &vendorA{baseVendor{vInfo}}
	case CodeT:
		return &vendorT{baseVendor{vInfo}}
	default:
		return &baseVendor{vInfo}
	}
}

type baseVendor struct {
	vInfo *VendorInfo
}

func (b *baseVendor) updateParams(req *WhipReq, token string) {
	req.url = fmt.Sprintf("%s/%s/%s/%s", b.vInfo.Domain, req.Action, b.vInfo.VAppId, req.StreamId)
	if req.HeaderParams == nil {
		req.HeaderParams = make(map[string]string)
	}
	req.HeaderParams[ContentType] = TypeSDP
	req.HeaderParams[Authorization] = "Bearer " + token

	if req.QueryParams == nil {
		req.QueryParams = make(map[string]string)
	}
	req.QueryParams["SessionID"] = req.SessionId

	if req.MuteAudio != nil {
		req.QueryParams["MuteAudio"] = strconv.FormatBool(*req.MuteAudio)
	}

	if req.MuteVideo != nil {
		req.QueryParams["MuteVideo"] = strconv.FormatBool(*req.MuteVideo)
	}

	if req.ClientIP != "" {
		req.QueryParams["ClientIP"] = req.ClientIP
	}

	if req.Action != actionPub && req.DisableDownlinkFb {
		req.QueryParams["DisableDownlinkFb"] = "true"
	}
}

func (b *baseVendor) GenWhipParams(ctx context.Context, req *WhipReq) error {
	claims := jwt.MapClaims{
		"exp":      time.Now().Add(time.Hour * 24).Unix(),
		"version":  "1.0",
		"appID":    b.vInfo.VAppId,
		"streamID": req.StreamId,
		"action":   req.Action,
	}

	if req.Action == actionPub {
		claims["enableSubAuth"] = true
	}

	raw := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	token, err := raw.SignedString([]byte(b.vInfo.Secret))
	if err != nil {
		return &WhipError{
			Code:      CodeBadRequest,
			Msg:       "failed to generate token",
			MsgOrigin: MsgOriginWhipsdk,
		}
	}

	b.updateParams(req, token)

	return nil
}

type vendorA struct {
	baseVendor
}

func (a *vendorA) GenWhipParams(ctx context.Context, req *WhipReq) error {
	err := a.baseVendor.GenWhipParams(ctx, req)
	if err != nil {
		return err
	}
	req.QueryParams["NoEncrypt"] = "1" // FIXME 声网暂时关闭加密
	return nil
}

type vendorT struct {
	baseVendor
}

func (t *vendorT) GenWhipParams(ctx context.Context, req *WhipReq) error {
	appID, err := strconv.Atoi(t.vInfo.VAppId)
	if err != nil {
		return &WhipError{
			Code:      http.StatusInternalServerError,
			Msg:       "failed to convert vendor app id " + t.vInfo.VAppId,
			MsgOrigin: MsgOriginWhipsdk,
		}
	}

	token, err := tencentyun.GenUserSig(appID, t.vInfo.Secret, req.SessionId, 86400)
	if err != nil {
		return &WhipError{
			Code:      http.StatusInternalServerError,
			Msg:       "failed to get user sig",
			MsgOrigin: MsgOriginWhipsdk,
		}
	}
	t.updateParams(req, token)

	return nil
}
