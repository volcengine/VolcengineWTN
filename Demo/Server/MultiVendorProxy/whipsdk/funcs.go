package whipsdk

import (
	"crypto/sha256"
	"encoding/base64"
	"math/rand"
	"time"

	"github.com/bytedance/sonic"
	"github.com/golang-jwt/jwt"
)

func InStringSlice(set []string, s string) bool {
	for _, v := range set {
		if v == s {
			return true
		}
	}

	return false
}

func init() {
	rand.Seed(time.Now().UnixNano())
}

func MarshalToString(v interface{}) string {
	b, _ := sonic.Marshal(v)
	return string(b)
}

func GenToken(params string, secret []byte) string {
	sha := sha256.New()
	sha.Write([]byte(params))
	hashVal := sha.Sum(nil)

	claims := jwt.MapClaims{
		"exp":           time.Now().Add(time.Hour * 24).Unix(),
		"version":       "1.0",
		claimsKeyParams: base64.StdEncoding.EncodeToString(hashVal),
	}

	raw := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	tokenStr, _ := raw.SignedString(secret)

	return tokenTypeBearer + " " + tokenStr
}
