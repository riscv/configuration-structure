#!/bin/bash

# Build C converter using asn1c

export CFLAGS=-Os

set -x
set -e

rm -f *.[ch]
asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler ../schema.asn
cp /usr/local/share/asn1c/OCTET_STRING_oer.c /usr/local/share/asn1c/BIT_STRING_oer.c .
cc -I. $CFLAGS -DPDU=Configuration -o converter *.c
size converter
