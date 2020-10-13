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

    for filename in args.filename:
        binary = open(filename, "rb").read()
        print("%s bytes:" % filename, len(binary))
        print("  Snappy compressed bytes:", len(snappy.compress(binary)))
        print("  Zlib compressed bytes:", len(zlib.compress(binary, 9)))
        print("  Lzma compressed bytes:", len(lzma.compress(binary)))

sys.exit(main())
