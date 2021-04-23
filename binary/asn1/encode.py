#!/usr/bin/python3

# Usage: encode.py < structure.jer > structure.uper

import asn1tools
import sys

uper = asn1tools.compile_files("schema.asn", "uper")
jer = asn1tools.compile_files("schema.asn", "jer")

tree = jer.decode('Configuration', sys.stdin.buffer.read())
sys.stdout.buffer.write(uper.encode('Configuration', tree))
