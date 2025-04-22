#!/bin/sh

CWD="$(dirname $0)"
FF_BINARY="$CWD/firefox"

exec env DYLD_INSERT_LIBRARIES="$HOME/.shimmy/shimmy.dylib" "$FF_BINARY" "$@"
