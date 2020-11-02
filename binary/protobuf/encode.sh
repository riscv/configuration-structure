#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cat $1 | protoc -I$SCRIPTPATH/schema --encode=Configuration $SCRIPTPATH/schema/main.proto > ${1%.*}-protobuf.bin
