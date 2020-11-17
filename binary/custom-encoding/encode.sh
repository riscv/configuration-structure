#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
TMP=`mktemp`

python3 $SCRIPTPATH/tool.py --schema $SCRIPTPATH/schema.json5 -c $1 \
    > $TMP \
    && mv $TMP ${1%.*}-custom.bin
