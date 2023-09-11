package whipsdk

import "proxy_server/whipsdk/logger"

// 业务注册获取vendorInfo的回调
func WithVendorInfoGetter(getter VendorGetter) Option {
	return func(o *options) {
		o.vendorInfoGetter = getter
	}
}

func WithSecret(secret []byte) Option {
	return func(o *options) {
		o.secret = secret
	}
}

func WithLogger(log logger.CtxLogger) Option {
	return func(o *options) {
		o.logger = log
	}
}

func WithProxyURL(proxyURL string) Option {
	return func(o *options) {
		o.proxyURL = proxyURL
	}
}

type VendorGetter func(rtcAppId string, vCode VendorCode) *VendorInfo

type Option func(o *options)

type options struct {
	vendorInfoGetter func(rtcAppId string, vCode VendorCode) *VendorInfo // 获取vendorInfo的回调，需业务传入
	secret           []byte
	logger           logger.CtxLogger
	proxyURL         string
}

type VendorInfo struct {
	VendorCode VendorCode `json:"vendorCode"`
	VAppId     string     `json:"vAppId"` // 绑定的厂商的appID
	Domain     string     `json:"domain"`
	Secret     string     `json:"secret"`
}
