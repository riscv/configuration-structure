#!/usr/bin/python3

# pylint: disable=missing-function-docstring
# pylint: disable=missing-module-docstring
# pylint: disable=fixme

import argparse
from argparse import RawTextHelpFormatter
import io
import json
import os.path
import sys
from pprint import pprint

import yaml

import asn1tools

ASN1TOOLS_FORMATS = ("jer", "uper", "xer", "ber")

def decode(schema_list, data, asn1_format):
    if asn1_format == "json":
        return json.loads(data)
    if asn1_format == "yaml":
        stream = io.BytesIO(data)
        return yaml.safe_load(stream)
    if asn1_format in ASN1TOOLS_FORMATS:
        asn1 = asn1tools.compile_files(schema_list, asn1_format)
        return asn1.decode('Top', data)
    raise ValueError("Unknown format: %r" % asn1_format)

def load(schema_list, path):
    asn1_format = os.path.splitext(path)[1][1:]
    data = open(path, "rb").read()
    return decode(schema_list, data, asn1_format)

def encode(schema_list, tree, asn1_format):
    if asn1_format == "json":
        return json.dumps(tree, indent=2).encode()
    if asn1_format == "yaml":
        return yaml.safe_dump(tree, indent=2).encode()
    if asn1_format in ASN1TOOLS_FORMATS:
        asn1 = asn1tools.compile_files(schema_list, asn1_format)
        return asn1.encode('Top', tree)
    raise ValueError("Unknown format: %r" % asn1_format)

def save(schema_list, path, tree):
    asn1_format = os.path.splitext(path)[1][1:]
    data = encode(schema_list, tree, asn1_format)
    open(path, "wb").write(data)

def cmd_convert(schema_list, args):
    tree = load(schema_list, args.source)

    # TODO: Can we error check the tree somehow?

    save(schema_list, args.destination, tree)

def all_values(obj):
    values = []
    if isinstance(obj, (list, tuple)):
        for value in obj:
            values.extend(all_values(value))
    elif isinstance(obj, dict):
        for k, value in obj.items():
            values.extend(all_values(k))
            values.extend(all_values(value))
    else:
        values.append(obj)
    return values

def cmd_test(schema_list, args):
    # This is not part of the standard python distribution, so only import it
    # when we might actually use it.
    import deepdiff # pylint: disable=import-outside-toplevel

    for path in args.path:
        original = load(schema_list, path)

        if path.endswith(".jer"):
            # When loading a .jer file, asn1tools silently ignores any entries
            # that are not mentioned in the schema. We don't want that, so load
            # the file as JSON and grab those entries for later comparison.
            original_plain = json.load(open(path, "rb"))
        else:
            original_plain = all_values(original)

        uper = encode(schema_list, original, "uper")
        result = decode(schema_list, uper, "uper")

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
    return 0

def schema_path_dict(schema_list):
    schema_dict = {}
    for schema in schema_list:
        path_elements = schema.split(os.path.sep)
        if path_elements[-1].endswith(".asn"):
            schema_dict[path_elements[-1]] = schema
    return schema_dict

def default_schema_dict():
    # Walk through ../schema for all standard ASN.1 schema files.
    default_schema_list = []
    for root, _, schemafiles in os.walk(os.path.join(
            os.path.dirname(os.path.realpath(__file__)), "..", "schema")):
        for schemafile in schemafiles:
            default_schema_list.append(root + os.path.sep + schemafile)
    return schema_path_dict(default_schema_list)

def additional_schema_dict(arg_schema):
    additional_schema_list = arg_schema.split(',')
    verified_list = []
    for additional_schema in additional_schema_list:
        # Validate the schema file.
        schema_file = os.path.normpath(
            os.path.join(os.path.dirname(os.path.realpath(__file__)), additional_schema))
        if os.path.exists(schema_file):
            verified_list.append(schema_file)
        else:
            print("Schema file: \"" + schema_file + "\" is not exist.")
    return schema_path_dict(verified_list)

SCHEMA_HELP_STRING = 'Specify the additional ASN.1 schema files to the standard\n' +\
                      'ones under /schema directory. Multiple ASN.1 schema files\n' +\
                      'could be specified using \',\' as the separator.\n' +\
                      'Example: --schema ../myschema1.asn,../myschema2.asn or\n'+\
                      '         --schema absolute_dir/myschema3.asn\n'
CONVERT_HELP_STRING = 'Convert between binary and human-readable formats.'
TEST_HELP_STRING = 'Test encoding/decoding the given files.'

def main():
    # Build up the default schema dictionary.
    default_schema = default_schema_dict()
    if len(default_schema) == 0:
        print("No default schemas found.")
        return 1

    parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument('--schema', help=SCHEMA_HELP_STRING)

    subparsers = parser.add_subparsers()

    parse_convert = subparsers.add_parser('convert', help=CONVERT_HELP_STRING)
    parse_convert.add_argument('source')
    parse_convert.add_argument('destination')
    parse_convert.set_defaults(func=cmd_convert)

    parse_test = subparsers.add_parser('test', help=TEST_HELP_STRING)
    parse_test.add_argument('path', nargs='+')
    parse_test.set_defaults(func=cmd_test)

    args = parser.parse_args()

    if args.schema:
        # Build up the dictionary of schemas assigned against to --schema option.
        schema_dictionary = {}

        # Generate the dictionary of additional schemas.
        schema_dictionary = additional_schema_dict(args.schema)
        if len(schema_dictionary) == 0:
            print("Can't find the schemas assigned to --schema option.")
            return 1
        # Combine schema_dictionary with the default schema.
        for default_schema_name in default_schema:
            if default_schema_name not in schema_dictionary:
                schema_dictionary[default_schema_name] = default_schema[default_schema_name]
    else:
        schema_dictionary = default_schema

    schema_list = list(schema_dictionary.values())

    return args.func(schema_list, args)

if __name__ == '__main__':
    sys.exit(main())
