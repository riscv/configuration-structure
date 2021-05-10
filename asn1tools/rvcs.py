#!/usr/bin/python3

import asn1tools
import argparse
import json
import os.path
import sys
import yaml

def cmd_convert(args):
    source_file = open(args.source, "rb")
    if args.source.endswith(".json"):
        tree = json.load(source_file)
    elif args.source.endswith(".yaml"):
        tree = yaml.load(source_file, Loader=yaml.FullLoader)
    elif args.source.endswith(".jer"):
        jer = asn1tools.compile_files(args.schema, "jer")
        tree = jer.decode('Configuration', source_file.read())
    elif args.source.endswith(".uper"):
        uper = asn1tools.compile_files(args.schema, "uper")
        tree = uper.decode('Configuration', source_file.read())

    # TODO: Can we error check the tree somehow?
    print(tree)

    if args.destination.endswith(".json"):
        data = json.dumps(tree, indent=2).encode()
    elif args.destination.endswith(".yaml"):
        data = yaml.safe_dump(tree, indent=2).encode()
    elif args.destination.endswith(".jer"):
        jer = asn1tools.compile_files(args.schema, "jer")
        data = jer.encode('Configuration', tree)
    elif args.destination.endswith(".uper"):
        uper = asn1tools.compile_files(args.schema, "uper")
        data = uper.encode('Configuration', tree)
    open(args.destination, "wb").write(data)

def main():
    default_schema = os.path.join(
        os.path.dirname(os.path.realpath(__file__)),
        "../schema.asn")

    parser = argparse.ArgumentParser()
    parser.add_argument('--schema', default=default_schema)

    subparsers = parser.add_subparsers()

    parse_encode = subparsers.add_parser('convert',
            help='Convert between binary and human-readable formats.')
    parse_encode.add_argument('source')
    parse_encode.add_argument('destination')
    parse_encode.set_defaults(func=cmd_convert)

    args = parser.parse_args()
    return args.func(args)

if __name__ == '__main__':
    sys.exit(main())
