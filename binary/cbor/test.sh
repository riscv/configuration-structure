#!/bin/sh
echo "- Remove generated instance.cbor"
rm -f instance.cbor

echo "- Generate example instance.json from spec.cddl as random-instance.json"
cddl spec.cddl json-generate > random-instance.json

echo "- Validate instance.json"
cddl spec.cddl validate instance.json

echo "- Convert instance.json to instance.cbor"
json2cbor.rb instance.json > instance.cbor

echo "- Dump instance.cbor as hex in instance.cbor.hex"
xxd instance.cbor > instance.cbor.hex

echo "- Compile CBOR parsing tool"
make -s parse-cbor

echo "- Parse CBOR and dump some information:"
./parse-cbor instance.cbor
