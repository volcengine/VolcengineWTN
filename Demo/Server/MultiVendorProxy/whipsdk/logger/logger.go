package logger

import (
	"context"
	"fmt"
	"os"
	"runtime"
	"strings"
	"time"
)

// ctxLogger接口
type CtxLogger interface {
	CtxDebugf(ctx context.Context, format string, v ...interface{})
	CtxInfof(ctx context.Context, format string, v ...interface{})
	CtxNoticef(ctx context.Context, format string, v ...interface{})
	CtxWarnf(ctx context.Context, format string, v ...interface{})
	CtxErrorf(ctx context.Context, format string, v ...interface{})
}

const (
	CtxLogId = "k_log_id"
)

func DefaultLogger() *simpleLogger {
	return &simpleLogger{}
}

type simpleLogger struct {
}

func (l *simpleLogger) CtxErrorf(ctx context.Context, format string, v ...interface{}) {
	defaultLogPrinter(ctx, "[Error] %s"+format, v...)
}
func (l *simpleLogger) CtxWarnf(ctx context.Context, format string, v ...interface{}) {
	defaultLogPrinter(ctx, "[Warn] %s "+format, v...)
}
func (l *simpleLogger) CtxNoticef(ctx context.Context, format string, v ...interface{}) {
	defaultLogPrinter(ctx, "[Notice] %s "+format, v...)
}
func (l *simpleLogger) CtxInfof(ctx context.Context, format string, v ...interface{}) {
	defaultLogPrinter(ctx, "[Info] %s "+format, v...)
}
func (l *simpleLogger) CtxDebugf(ctx context.Context, format string, v ...interface{}) {
	defaultLogPrinter(ctx, "[Debug] %s "+format, v...)
}

func defaultLogPrinter(ctx context.Context, format string, v ...interface{}) {
	_, file, line, _ := runtime.Caller(2)
	location := fmt.Sprintf("%s %s:%d logid:%s ", time.Now().String(), GetRootDir(file), line, ctx.Value(CtxLogId))
	args := append([]interface{}{location}, v...)
	fmt.Printf(format+"\n", args...)
}

func GetRootDir(file string) string {
	dir, _ := os.Getwd()
	return strings.Replace(file, dir, "", -1)
}
