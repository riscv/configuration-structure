#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
TMP=`mktemp`

protoc -I$SCRIPTPATH/schema --encode=Configuration $SCRIPTPATH/schema/main.proto \
    < $1 \
    > $TMP \
    && mv $TMP ${1%.*}-protobuf.bin
