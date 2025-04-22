#!/bin/sh

CWD="$(dirname $0)"

# Wrap the Firefox binary
sudo xattr -cr /Applications/Firefox.app
sudo codesign -fs - /Applications/Firefox.app/Contents/MacOS/firefox

cp "$CWD/wrappers/firefox-wrapper-macos.sh" /Applications/Firefox.app/Contents/MacOS/firefox-wrapper

mkdir -p $HOME/.shimmy
cp "$CWD/../target/c/shimmy.dylib" $HOME/.shimmy/shimmy.dylib

sed -i '' 's|<string>firefox</string>|<string>firefox-wrapper</string>|' /Applications/Firefox.app/Contents/Info.plist
