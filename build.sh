#!/bin/bash

if [[ $CC = "" ]]; then
    CC=clang
fi

IS_RELEASE=false

if [[ $OPT_LEVEL = "" ]]; then
    if [[ $1 = "release" ]]; then
        OPT_LEVEL=O3
    else
        OPT_LEVEL=O0
    fi
fi

if [[ $CC_FLAGS = "" ]]; then
    if [[ $1 = "release" ]]; then
        CC_FLAGS="-Wall -Wextra -std=c11 -DNDEBUG -Wno-unused-command-line-argument"
    else
        CC_FLAGS="-Wall -Wextra -std=c11 -g -Wno-unused-command-line-argument"
    fi
fi

TARGET_DIR=./target
C_TARGET_DIR=$TARGET_DIR/c
INCLUDE_DIR=./cinclude
LIB_NAME=shimmy

SRC_FILES=./src/syscall_intercept.c

if [[ !($1 = "clean" || $1 = "release" || $1 = "") ]]; then
    echo "Usage: ./$(basename $0) <release|clean>"
    exit 1
fi

if [[ $1 = "clean" ]]; then
    rm -rf $C_TARGET_DIR && cargo clean
    exit 0
fi

function build_objs {
    if [[ $1 = "release" ]]; then
        RS_LIB_PATH=$TARGET_DIR/release/libshimmy.a
        (PS4="\000" set -x; cargo build --release) || exit 1
    else
        RS_LIB_PATH=$TARGET_DIR/debug/libshimmy.a
        (PS4="\000" set -x; cargo build) || exit 1
    fi

    OUT=$C_TARGET_DIR
    mkdir -p $OUT

    for FILE in $SRC_FILES; do
        FNAME=$(basename $FILE)
        FILE_NO_EXT="${FNAME%.*}"
        OBJ="$OUT/$FILE_NO_EXT.o"
        (PS4="\000" set -x; $CC -$OPT_LEVEL $CC_FLAGS -c $FILE -o $OBJ -I$INCLUDE_DIR) || exit 1

        if [[ $? -ne 0 ]]; then
            exit 1
        fi
    done

    (PS4="\000" set -x; $CC -shared -o $OUT/$LIB_NAME.dylib $OUT/*.o $RS_LIB_PATH -dynamiclib -fPIC -fvisibility=hidden)
}

build_objs $1
