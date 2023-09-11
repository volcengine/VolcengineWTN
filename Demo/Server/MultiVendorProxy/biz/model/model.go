package model

type ProxyReq struct {
	Params  string `json:"params,required"`
	Token   string `header:"Authorization,required"`
	TraceId string `header:"X-Tt-Traceid"`
}
