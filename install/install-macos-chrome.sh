#!/bin/sh

CWD="$(dirname $0)"

# Wrap the Chrome binary
sudo xattr -cr /Applications/Google\ Chrome.app
sudo codesign -fs - /Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome
cp "$CWD/wrappers/chrome-wrapper-macos.sh" /Applications/Google\ Chrome.app/Contents/MacOS/chrome-wrapper
cp "$CWD/../target/c/shimmy.dylib" /Applications/Google\ Chrome.app/Contents/MacOS/shimmy.dylib
sed -i '' -e '/<key>CFBundleExecutable<\/key>/{n;s|<string>.*</string>|<string>chrome-wrapper</string>|;}' /Applications/Google\ Chrome.app/Contents/Info.plist
