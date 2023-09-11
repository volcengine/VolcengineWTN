package whipsdk

import (
	"context"
	"net/http"
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_GenWhipParams(t *testing.T) {
	v := newVendor(&VendorInfo{})
	assert.NotNil(t, v)

	vi := &VendorInfo{
		VendorCode: CodeA,
		VAppId:     "a_appid",
		Domain:     "http://www.vendora.com",
		Secret:     "secret",
	}
	_vendorA := newVendor(vi)

	mute := true

	ctx := context.Background()
	info := WhipReq{
		VendorCode:        CodeA,
		AppId:             "rtcappId",
		SDP:               "",
		MuteAudio:         &mute,
		MuteVideo:         &mute,
		ClientIP:          "1.1.1.1",
		Action:            actionPub,
		StreamId:          "streamId",
		SessionId:         "sid",
		DisableDownlinkFb: true,
		QueryParams:       nil,
		HeaderParams:      nil,
		url:               "",
	}
	err := _vendorA.GenWhipParams(ctx, &info)
	assert.Nil(t, err)
	assert.Equal(t, "1", info.QueryParams["NoEncrypt"])
	assert.Equal(t, "true", info.QueryParams["MuteAudio"])
	assert.Equal(t, "true", info.QueryParams["MuteVideo"])
	assert.Equal(t, "sid", info.QueryParams["SessionID"])
	assert.Equal(t, "1.1.1.1", info.QueryParams["ClientIP"])
	assert.Equal(t, "", info.QueryParams["DisableDownlinkFb"])
	assert.True(t, info.HeaderParams[Authorization] != "")
	assert.True(t, info.HeaderParams["Content-Type"] == "application/sdp")

	info.Action = actionSub
	_vendorA.GenWhipParams(ctx, &info)
	assert.Equal(t, "true", info.QueryParams["DisableDownlinkFb"])

	vi = &VendorInfo{
		VendorCode: CodeT,
		VAppId:     "t_appid",
		Domain:     "http://www.vendort.com",
		Secret:     "secret",
	}
	info.VendorCode = CodeT
	info.QueryParams = nil
	info.HeaderParams = nil
	info.SessionId = "xxxsid"

	_vendorT := newVendor(vi)
	err = _vendorT.GenWhipParams(ctx, &info)
	assert.NotNil(t, err)
	code, msg, origin := GetCodeAndMessage(err)
	assert.Equal(t, http.StatusInternalServerError, code)
	assert.Equal(t, MsgOriginWhipsdk, origin)
	assert.Contains(t, msg, "ailed to convert vendor app id")

	vi.VAppId = "123"
	err = _vendorT.GenWhipParams(ctx, &info)
	assert.Nil(t, err)
	assert.Equal(t, "", info.QueryParams["NoEncrypt"])
	assert.Equal(t, "true", info.QueryParams["MuteAudio"])
	assert.Equal(t, "true", info.QueryParams["MuteVideo"])
	assert.Equal(t, "true", info.QueryParams["MuteVideo"])
	assert.Equal(t, "xxxsid", info.QueryParams["SessionID"])
	assert.Equal(t, "1.1.1.1", info.QueryParams["ClientIP"])
	assert.True(t, info.HeaderParams[Authorization] != "")
	assert.True(t, info.HeaderParams["Content-Type"] == "application/sdp")

}
