#!/usr/bin/python3

import argparse
import asn1tools
import collections
import io
import json
import os.path
import sys
import yaml
from pprint import pprint

asn1tools_formats = ("jer", "uper", "xer", "ber")

def decode(schema, bytes, format):
    if format == "json":
        return json.loads(bytes)
    elif format == "yaml":
        stream = io.BytesIO(bytes)
        return yaml.safe_load(stream)
    elif format in asn1tools_formats:
        asn1 = asn1tools.compile_files(schema, format)
        return asn1.decode('Configuration', bytes)
    else:
        raise ValueError("Unknown format: %r" % format)

def load(schema, path):
    format = os.path.splitext(path)[1][1:]
    bytes = open(path, "rb").read()
    return decode(schema, bytes, format)

def encode(schema, tree, format):
    if format == "json":
        return json.dumps(tree, indent=2).encode()
    elif format == "yaml":
        return yaml.safe_dump(tree, indent=2).encode()
    elif format in asn1tools_formats:
        asn1 = asn1tools.compile_files(schema, format)
        return asn1.encode('Configuration', tree)
    else:
        raise ValueError("Unknown format: %r" % format)

def save(schema, path, tree):
    format = os.path.splitext(path)[1][1:]
    bytes = encode(schema, tree, format)
    open(path, "wb").write(bytes)

def cmd_convert(args):
    tree = load(args.schema, args.source)

    # TODO: Can we error check the tree somehow?

    save(args.schema, args.destination, tree)

def all_values(object):
    values = []
    if isinstance(object, list) or isinstance(object, tuple):
        for v in object:
            values.extend(all_values(v))
    elif isinstance(object, dict):
        for k, v in object.items():
            values.extend(all_values(k))
            values.extend(all_values(v))
    else:
        values.append(object)
    return values

def cmd_test(args):
    import deepdiff

    for path in args.path:
        original = load(args.schema, path)

        if path.endswith(".jer"):
            # When loading a .jer file, asn1tools silently ignores any entries
            # that are not mentioned in the schema. We don't want that, so load
            # the file as JSON and grab those entries for later comparison.
            original_plain = json.load(open(path, "rb"))
        else:
            original_plain = all_values(original)

        uper = encode(args.schema, original, "uper")
        result = decode(args.schema, uper, "uper")

        # Check for differences between the decoded input and the result.
        difference = deepdiff.DeepDiff(original, result)
        if difference:
            print("Final result does not match original %s:" % path)
            pprint(difference)
            return 1

        # Check for differences between the plain data and the result.
        plain_difference = deepdiff.DeepDiff(original_plain, result)
        if 'dictionary_item_added' in plain_difference:
            # Items added in the decode step is OK. That happens when there are
            # items with a default value that were not specified in the input.
            del plain_difference['dictionary_item_added']
        if 'type_changes' in plain_difference:
            # Types change between reading a file as JSON and as JER, so this is
            # expected.
            del plain_difference['type_changes']
        if plain_difference:
            print("Final result does not match plain original %s:" % path)
            pprint(plain_difference)
            return 1

        print("%s is %dB in UPER" % (path, len(uper)))

def main():
    default_schema = os.path.join(
        os.path.dirname(os.path.realpath(__file__)),
        "../schema.asn")

    parser = argparse.ArgumentParser()
    parser.add_argument('--schema', default=default_schema)

    subparsers = parser.add_subparsers()

    parse_convert = subparsers.add_parser('convert',
            help='Convert between binary and human-readable formats.')
    parse_convert.add_argument('source')
    parse_convert.add_argument('destination')
    parse_convert.set_defaults(func=cmd_convert)

    parse_test = subparsers.add_parser('test',
            help='Test encoding/decoding the given files.')
    parse_test.add_argument('path', nargs='+')
    parse_test.set_defaults(func=cmd_test)

    args = parser.parse_args()
    return args.func(args)

if __name__ == '__main__':
    sys.exit(main())
