#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

python3 $SCRIPTPATH/tool.py --schema $SCRIPTPATH/schema.json5 -c $1 > ${1%.*}-custom.bin
