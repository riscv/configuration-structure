#!/bin/sh

cat $1 | protoc -Ischema --encode=Configuration schema/main.proto > ${1%.*}-protobuf.bin
