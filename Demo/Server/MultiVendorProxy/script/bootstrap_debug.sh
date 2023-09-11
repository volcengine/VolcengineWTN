
#!/bin/bash
CURDIR=$(cd $(dirname $0); pwd)
BinaryName=proxy_server
echo "$CURDIR/bin/${BinaryName}"
EXE_FILE=$CURDIR/bin/${BinaryName}

go build -v -gcflags "all=-N -l" -ldflags "-X 'main.GitSHA=${GIT_SHA}' -X 'main.BuildTime=${BUILD_TIME}'" -o $EXE_FILE

echo "==============================================="
echo "================ launch debug ================="
echo "==============================================="
echo "dlv --listen=:2345 --headless=true --api-version=2 exec ${EXE_FILE} -- $args"
dlv --listen=:2345 --headless=true --api-version=2 exec ${EXE_FILE} -- $args
