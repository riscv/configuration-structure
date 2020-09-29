# Protobuf

This is a Proto3 example.

The schema is in the schema directory. `encode.sh` with a YAML description will
turn that YAML into a binary representation. `decode.sh` with a binary file
will turn it into some human-readable format.

`make` will generate python code to work with the binary representation.
