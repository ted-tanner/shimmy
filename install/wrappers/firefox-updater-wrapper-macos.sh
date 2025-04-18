#!/bin/sh

CWD="$(dirname $0)"
UPDATER_BINARY="$CWD/org.mozilla.updater"

exec "$UPDATER_BINARY" "$@"

./install-macos.sh
