# Protobuf

This is a Proto3 example, just like `../example.md`
`example.proto` contains the schema, `example` the example data.
By running `cat example | protoc --encode=Configuration > example.binary` you can dump
the example as binary. The hex version of that is available in example.hex.
The binary size is 196.

Run `make` to generate python code to convert `example.proto` into Python
code, and then you can run `example.py` to examine its output.
