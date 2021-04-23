#!/usr/bin/python3

# Usage: decode.py < structure.uper > structure.jer

import asn1tools
import sys

uper = asn1tools.compile_files("schema.asn", "uper")
jer = asn1tools.compile_files("schema.asn", "jer")

tree = uper.decode('Configuration', sys.stdin.buffer.read())
sys.stdout.buffer.write(jer.encode('Configuration', tree, indent=4))
