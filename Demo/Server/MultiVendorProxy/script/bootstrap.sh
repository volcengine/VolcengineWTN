
#!/bin/bash
CURDIR=$(cd $(dirname $0); pwd)
BinaryName=proxy_server
echo "$CURDIR/bin/${BinaryName}"
exec $CURDIR/bin/${BinaryName}
