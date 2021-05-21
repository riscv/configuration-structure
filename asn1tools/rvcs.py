#!/usr/bin/python3

import argparse
import asn1tools
import collections
import json
import os.path
import sys
import tempfile
import yaml
from pprint import pprint

def load(schema, path, format=None):
    file = open(path, "rb")
    if format is None:
        format = os.path.splitext(path)[-1][1:]
    if format == "json":
        tree = json.load(file)
    elif format == "yaml":
        tree = yaml.load(file, Loader=yaml.FullLoader)
    elif format == "jer":
        jer = asn1tools.compile_files(schema, "jer")
        tree = jer.decode('Configuration', file.read())
    elif format == "uper":
        uper = asn1tools.compile_files(schema, "uper")
        tree = uper.decode('Configuration', file.read())
    else:
        raise ValueError("Unknown format: %r" % format)
    return tree

def save(schema, path, tree):
    if path.endswith(".json"):
        data = json.dumps(tree, indent=2).encode()
    elif path.endswith(".yaml"):
        data = yaml.safe_dump(tree, indent=2).encode()
    elif path.endswith(".jer"):
        jer = asn1tools.compile_files(schema, "jer")
        data = jer.encode('Configuration', tree)
    elif path.endswith(".uper"):
        uper = asn1tools.compile_files(schema, "uper")
        data = uper.encode('Configuration', tree)
    open(path, "wb").write(data)

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
    for path in args.path:
        original = load(args.schema, path)

        if path.endswith(".jer"):
            # When loading a .jer file, asn1tools silently ignores any entries
            # that are not mentioned in the schema. We don't want that, so load
            # the file as JSON and grab those entries for later comparison.
            original_values = all_values(json.load(open(path, "rb")))
        else:
            original_values = all_values(original)
        original_counted = collections.Counter(original_values)

        tmp_uper = tempfile.NamedTemporaryFile(suffix=".uper")
        save(args.schema, tmp_uper.name, original)
        result = load(args.schema, tmp_uper.name)
        result_values = all_values(result)
        result_counted = collections.Counter(result_values)

        if original != result:
            import deepdiff
            pprint(deepdiff.DeepDiff(original, result))
            return 1

        if original_counted != result_counted:
            import deepdiff
            print("Difference in original vs. result values:")
            pprint(deepdiff.DeepDiff(original_counted, result_counted))
            print("This can happen if the input contains entries that are not ")
            print("part of the schema.")
            return 1

        print("%s is %dB in UPER" % (path,
                                     os.path.getsize(tmp_uper.name)))

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
