#!/bin/bash

if [[ $CC = "" ]]; then
    CC=clang
fi

IS_RELEASE=false

if [[ $OPT_LEVEL = "" ]]; then
    if [[ $1 = "release" ]]; then
        OPT_LEVEL=O3
    elif [[ $1 = "test" && $2 = "release" ]]; then
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
LIB_NAME=shimmy

SRC_DIR=./src
INCLUDE_DIR=./include
TEST_SRC_DIR=./tests

SRC_FILES=$(echo $(find $SRC_DIR -type f -name "*.c") $(find src -type f -name "*.S"))

TEST_SRC_FILES=$(echo $(find $TEST_SRC_DIR -maxdepth 1 -type f -name "*.c" -print0 | sort -z | xargs -0))

if [[ !($1 = "clean" || $1 = "test" || $1 = "release" || $1 = "") ]]; then
    echo "Usage: ./$(basename $0) <clean|test|test release|release>"
    exit 1
fi

if [[ $1 = "clean" ]]; then
    rm -rf $TARGET_DIR
    exit 0
fi

function build_objs {
    if [[ $1 = "test" ]]; then
        MACROS="-DSHMY_DEBUG"
    else
        MACROS=""
    fi

    OUT="$TARGET_DIR"
    mkdir -p $OUT

    for FILE in $SRC_FILES; do
        FNAME=$(basename $FILE)
        FILE_NO_EXT="${FNAME%.*}"
        OBJ=" $OUT/$FILE_NO_EXT.o"
        (PS4="\000" set -x; $CC -$OPT_LEVEL $MACROS $CC_FLAGS -c $FILE -o $OBJ -I$INCLUDE_DIR) || exit 1

        if [[ $? -ne 0 ]]; then
            exit 1
        fi
    done

    (PS4="\000" set -x; $CC -shared -o $OUT/$LIB_NAME.dylib $OUT/*.o -dynamiclib -fPIC -fvisibility=hidden)
}

function file_to_test_name {
    TEST_FILE_NAME=$(basename $1)
    echo "${TEST_FILE_NAME%.*}"
}

function build_test {
    TEST_OUT="$TARGET_DIR/tests"

    FILES="$TARGET_DIR/$LIB_NAME.dylib $1"
    MACROS="-DSHMY_DEBUG"

    OUT_FILE=$TEST_OUT/$(file_to_test_name $1)

    mkdir -p $TEST_OUT

    (PS4="\000" set -x; $CC -$OPT_LEVEL $MACROS $CC_FLAGS $FILES -o $OUT_FILE -I$INCLUDE_DIR -L$TARGET_DIR)
}

build_objs $1 &&

if [[ $1 = "test" ]]; then
    TEST_NAMES=()

    for FILE in $TEST_SRC_FILES; do
        TEST_NAMES+=" $(file_to_test_name $FILE)"
        build_test $FILE
    done &&

    SUCCESS_COUNT=0 &&
    TEST_COUNT=0 &&
    echo "" &&

    for TEST_NAME in $TEST_NAMES; do
        echo "----- Running test: $TEST_NAME -----"
        echo ""
        time $TARGET_DIR/tests/$TEST_NAME

        if [[ $? -eq 0 ]]; then
            SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
        fi

        TEST_COUNT=$((TEST_COUNT + 1))
    done &&

    echo "" &&
    echo "------------------" &&
    echo "PASSED: $SUCCESS_COUNT test set(s)" &&
    echo "FAILED: $((TEST_COUNT-SUCCESS_COUNT)) test set(s)" &&
    echo "------------------"
fi
