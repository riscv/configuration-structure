#!/bin/bash

export CFLAGS=-Os

set -x

rm -r build
set -e
mkdir build
cd build
asn1c ../schema.asn1
cp /usr/local/share/asn1c/OCTET_STRING_oer.c /usr/local/share/asn1c/BIT_STRING_oer.c .
cc -I. $CFLAGS -DPDU=Configuration -o converter *.c
size converter
