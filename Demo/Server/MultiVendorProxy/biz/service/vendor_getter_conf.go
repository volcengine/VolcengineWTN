package service

import (
	"strconv"

	"proxy_server/whipsdk"

	"github.com/bytedance/sonic"
)

type Vendors map[string]map[string]*whipsdk.VendorInfo

var vendorsMap Vendors

func init() {
	// vendorconf
	// conf: key(appid) => value(vendorInfo)
	// vendorInfo: key(vendorCode) => value(info)
	conf := `{
		"default":{
			"1": {
				"vendorCode":1,
				"vAppId":"appid",
				"domain":"https://www.whipserver.com",
				"secret":""
			}
		}
	}`

	vendorsMap = make(Vendors)
	sonic.Unmarshal([]byte(conf), &vendorsMap)
}

func Getter(rtcAppId string, vCode whipsdk.VendorCode) *whipsdk.VendorInfo {

	return confGetter(rtcAppId, vCode)
}

func confGetter(rtcAppId string, vCode whipsdk.VendorCode) *whipsdk.VendorInfo {
	vInfos, ok := vendorsMap[rtcAppId]
	if ok {
		if v, ok := vInfos[strconv.Itoa(int(vCode))]; ok {
			return v
		}
	}

	vInfos, ok = vendorsMap["default"]
	return vInfos[strconv.Itoa(int(vCode))]
}
