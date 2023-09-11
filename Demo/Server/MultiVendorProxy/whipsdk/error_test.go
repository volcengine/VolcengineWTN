package whipsdk

import (
	"errors"
	"net/http"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGetCodeAndMessage(t *testing.T) {
	code, msg, origin := GetCodeAndMessage(&WhipError{
		Code:      210,
		Msg:       "msg",
		MsgOrigin: MsgOriginWhipsdk,
	})

	assert.Equal(t, 210, code)
	assert.Equal(t, msg, msg)
	assert.Equal(t, MsgOriginWhipsdk, origin)

	code, msg, origin = GetCodeAndMessage(errors.New("normal err"))
	assert.Equal(t, http.StatusInternalServerError, code)
	assert.Equal(t, "normal err", msg)
	assert.Equal(t, MsgOriginWhipserver, origin)

	code, msg, origin = GetCodeAndMessage(nil)
	assert.Equal(t, http.StatusOK, code)
	assert.Equal(t, "success", msg)
	assert.Equal(t, MsgOriginWhipserver, origin)
}
