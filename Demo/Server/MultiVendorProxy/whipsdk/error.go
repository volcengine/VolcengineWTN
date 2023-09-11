package whipsdk

import (
	"errors"
	"fmt"
	"net/http"
)

const (
	ErrTokenMissing     = "Token Missing"
	ErrTokenIllegal     = "Token Illegal"
	ErrTokenCheckFailed = "Token Check Failed"
	ErrTokenExpired     = "Token Expired"
	ErrBadRequest       = "Bad Request"
)

const (
	CodeTokenInvalid = 401
	CodeBadRequest   = 400
)

const (
	tokenTypeBearer = "Bearer"
)

const (
	// response信息由哪里产生，为空表示业务方服务
	RespHeaderMsgOrigin = "X-Tt-Msg-Origin"
)

type MsgOrigin string

var (
	MsgOriginWhipsdk    MsgOrigin = "whipsdk"
	MsgOriginWhipserver MsgOrigin = "whipserver"
)

type WhipError struct {
	Code      int
	Msg       string
	MsgOrigin MsgOrigin
}

func (e *WhipError) Error() string {
	return fmt.Sprintf("[%s]code:%d, msg:%s", e.MsgOrigin, e.Code, e.Msg)
}

func GetCodeAndMessage(err error) (code int, msg string, origin MsgOrigin) {
	wErr := &WhipError{}
	if errors.As(err, &wErr) {
		return wErr.Code, wErr.Msg, wErr.MsgOrigin
	} else if err != nil {
		return http.StatusInternalServerError, err.Error(), MsgOriginWhipserver
	} else {
		return http.StatusOK, "success", MsgOriginWhipserver
	}
}
