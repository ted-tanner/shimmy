#!/bin/sh

CWD="$(dirname $0)"
CHROME_BINARY="$CWD/Google Chrome"

exec env DYLD_INSERT_LIBRARIES="$CWD/shimmy.dylib" "$CHROME_BINARY" "$@"
