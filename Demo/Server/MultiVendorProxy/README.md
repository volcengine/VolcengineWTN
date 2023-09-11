# 代理服务器

## 简介
代码结构如下
```go
.
├── README.md
├── biz
│   ├── handler
│   │   ├── ping.go
│   │   └── proxy.go
│   ├── model
│   │   └── model.go
│   ├── router
│   │   └── register.go
│   └── service
│       ├── vendor_getter_conf.go
├── build.sh
├── go.mod
├── go.sum
├── main.go
├── router.go
├── router_gen.go
├── script
│   └── bootstrap.sh
└── whipsdk         
    ├── README.md
    ├── error.go
    ├── error_test.go
    ├── funcs.go
    ├── logger
    │   └── logger.go
    ├── options.go
    ├── vendor.go
    ├── vendor_test.go
    ├── whip.go
    └── whip_test.go
```
服务代码是由开源框架hertz生成，其中有两个部分
* Web框架: 后续可根据业务实际情况替换即可
* SDK: 包whipsdk须业务方引入到本地代码仓库中

## Web框架参考
https://www.cloudwego.io/docs/hertz/overview/

## 接口
### 1. 心跳检测 /whip/ping
#### 方法: GET
#### 返回:
| 字段  |     类型 |       说明      | 
|-----|-------:|:-------------:|
| -   | string | text/plain; charset=utf-8 |

### 1. 代理请求 /whip/proxy
#### 方法: POST
#### 请求头:
| 字段               |          类型 / 示例 |                     说明                    | 
|------------------|-----------------:|:-----------------------------------------:|
| Authorization    | string |  使用在火山引擎上设置的secret生成的token， Bearer xxxxx  |
| Content-Type     | application/json |                                           |
| X-Tt-Traceid     | string |        traceid，便于定位请求日志    |

#### 响应头:
| 字段              |          类型 / 示例 |                    说明                     | 
|-----------------|-----------------:|:-----------------------------------------:|
| X-Tt-Msg-Origin | string | 信息由哪里产生，枚举值有：whipsdk、whipserver。为空表示业务方服务 |
| 其他              |  |                透传厂商返回的头信息                 |

#### 请求体Body:
| 字段              |          类型 / 示例 |                    说明                    | 
|-----------------|-----------------:|:----------------------------------------:|
| params | string | 包含了操作类型、操作参数等所有信息的 json 字符串 |

params结构如下
```go
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
}
```


## SDK 使用说明(详细见bin/handler/proxy.go)
```go
var (
	secret    = []byte("YOUR SECRET")
	log       = logger.DefaultLogger()
	whipProxy whipsdk.WhipProxy
)

func init() {

	whipProxy = whipsdk.NewWhipProxy(
		whipsdk.WithSecret(secret),
		whipsdk.WithVendorInfoGetter(service.Getter),
		// 设置日志组件
		whipsdk.WithLogger(log),
	)
}

// Proxy .
func Proxy(ctx context.Context, c *app.RequestContext) {
	// your code ...

	req := model.ProxyReq{}
	err := c.BindAndValidate(&req)
	if err != nil {
		log.CtxErrorf(context.TODO(), "bad request:%s", string(c.Request.Body()))
		c.JSON(http.StatusBadRequest, "params is invalid")
		return
	}

	// 记录请求者的日志ID
	ctx = context.WithValue(ctx, logger.CtxLogId, req.TraceId)
	defer func(start time.Time) {
		log.CtxInfof(ctx, "[cost:%s] proxy req:%+v", time.Since(start).String(), req)
	}(time.Now())

	wReq, err := whipProxy.CheckToken(ctx, req.Params, req.Token)
	if err != nil {
		BuildErrResp(c, err)
		return
	}

	resp, err := whipProxy.SendWhip(ctx, wReq)
	if err != nil {
		BuildErrResp(c, err)
		return
	}
	BuildSuccResp(c, resp)
}

// 构建错误输出 （必要）
func BuildErrResp(c *app.RequestContext, err error) {
	code, msg, origin := whipsdk.GetCodeAndMessage(err)
	c.Response.Header.Set(whipsdk.RespHeaderMsgOrigin, string(origin))
	c.String(code, msg)
}

// 透传产商返回信息，包括header头 (必要)
func BuildSuccResp(c *app.RequestContext, resp *whipsdk.WhipResp) {
	for k, v := range resp.Headers {
		for _, val := range v {
			c.Response.Header.Add(k, val)
		}
	}

	c.Data(resp.StatusCode, c.Response.Header.Get("Content-Type"), []byte(resp.Body))
}
```


## Getter - 使sdk可以用来获取厂商信息的接口
接口定义
```go
type VendorGetter func(rtcAppId string, vCode whipsdk.VendorCode) *whipsdk.VendorInfo
```

## 编译&运行
进入到项目根目录
```shell
# 编译
$ sh build.sh

# 编译之后，会在当前目录下生成一个目录output,执行如下命令启动服务
$ output/bootstrap.sh

xxxx/xx/xx 17:02:15.057032 transport.go:109: [Info] HERTZ: HTTP server listening on address=[::]:8789

```

## 访问
默认，端口为8789
```shell
curl http://127.0.0.1:8789/whip/ping
```




