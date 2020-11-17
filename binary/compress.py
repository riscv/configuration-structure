#!/usr/bin/python3

import sys
import argparse
import snappy
import zlib
import lzma

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', nargs='+')
    args = parser.parse_args()

    fmt = "%-45s%8s%8s%8s%8s"
    print(fmt % ("name", "size", "snappy", "zlib", "lzma"))
    for filename in args.filename:
        binary = open(filename, "rb").read()
        print(fmt % (filename, len(binary),
            len(snappy.compress(binary)),
            len(zlib.compress(binary)),
            len(lzma.compress(binary))))

sys.exit(main())
