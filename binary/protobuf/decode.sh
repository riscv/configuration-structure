#!/bin/sh

cat $1 | protoc -Ischema --decode=Configuration schema/main.proto
