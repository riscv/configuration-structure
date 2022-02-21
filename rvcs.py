#!/usr/bin/python3

# pylint: disable=missing-function-docstring
# pylint: disable=missing-module-docstring
# pylint: disable=fixme

import argparse
from argparse import RawTextHelpFormatter
import io
import os.path
import sys
from pprint import pprint

import json
import json5
import yaml
import toml

import asn1tools

ASN1TOOLS_FORMATS = ("jer", "uper", "xer", "ber")

def compile_files(schema_list, asn1_format):
    try:
        return asn1tools.compile_files(schema_list, asn1_format)
    except asn1tools.parser.ParseError:
        # Parse each file in turn in case there's a syntax error, so we can tell the
        # user which file has the problem.
        for path in schema_list:
            f_contents = open(path).read()
            try:
                asn1tools.compile_string(f_contents)
            except asn1tools.parser.ParseError as exception:
                print("While parsing %s:" % path)
                print(exception)
                sys.exit(1)
        assert 0

def decode_normalize(schema_list, data):
    """Normalize output of a parser that is not aware of the ASN.1 schema by
    turning it into JSON and then parsing it as JER."""
    json_text = json.dumps(data).encode()
    asn1 = compile_files(schema_list, "jer")
    return asn1.decode('Top', json_text)

def decode(schema_list, data, asn1_format):
    if asn1_format == "json":
        return decode_normalize(schema_list, json.loads(data))
    if asn1_format == "json5":
        return decode_normalize(schema_list, json5.loads(data))
    if asn1_format == "asn":
        # Lazy import this module which is not commonly installed.
        import asn1vnparser # pylint: disable=import-outside-toplevel
        return decode_normalize(schema_list, asn1vnparser.parse_asn1_value(data.decode()))
    if asn1_format == "yaml":
        stream = io.BytesIO(data)
        return decode_normalize(schema_list, yaml.safe_load(stream))
    if asn1_format == "toml":
        return decode_normalize(schema_list, toml.loads(data.decode()))
    if asn1_format in ASN1TOOLS_FORMATS:
        asn1 = compile_files(schema_list, asn1_format)
        return asn1.decode('Top', data)

    raise ValueError("Don't know how to decode %r" % asn1_format)

def load(schema_list, path):
    asn1_format = os.path.splitext(path)[1][1:]
    data = open(path, "rb").read()
    return decode(schema_list, data, asn1_format)

def encode_normalize(schema_list, tree):
    """Normalize the parsed tree into a format that can be exported by a dumper
    that is not aware of the ASN.1 schema by turning it into JER and then
    parsing that as JSON."""
    asn1 = compile_files(schema_list, "jer")
    jer_text = asn1.encode('Top', tree)
    return json.loads(jer_text)

def encode(schema_list, tree, asn1_format):
    if asn1_format == "json":
        return json.dumps(encode_normalize(schema_list, tree), indent=2).encode()
    if asn1_format == "json5":
        return json5.dumps(encode_normalize(schema_list, tree), indent=2).encode()
    if asn1_format == "yaml":
        return yaml.safe_dump(encode_normalize(schema_list, tree), indent=2).encode()
    if asn1_format == "toml":
        return toml.dumps(encode_normalize(schema_list, tree)).encode()
    if asn1_format in ASN1TOOLS_FORMATS:
        asn1 = compile_files(schema_list, asn1_format)
        return asn1.encode('Top', tree)
    raise ValueError("Don't know how to encode to %r" % asn1_format)

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

def check_plain_difference(old, new):
    # This is not part of the standard python distribution, so only import it
    # when we might actually use it.
    import deepdiff # pylint: disable=import-outside-toplevel

    # Check for differences between the plain data and the result.
    difference = deepdiff.DeepDiff(old, new)
    if 'dictionary_item_added' in difference:
        # Items added in the decode step is OK. That happens when there are
        # items with a default value that were not specified in the input.
        del difference['dictionary_item_added']
    if 'type_changes' in difference:
        acceptable_differences = []
        for root, delta in difference['type_changes'].items():
            if delta['old_type'] == dict and delta['new_type'] == tuple:
                # This can be the difference between reading a file as JSON and
                # as JER, so could be OK.
                plain_difference = check_plain_difference(
                    delta['old_value'], dict([delta['new_value']]))
                if plain_difference:
                    # Just return right here. We don't print an exhaustive list
                    # of differences, which can be hard to parse. Instead just
                    # give the user something to improve on.
                    return plain_difference

                acceptable_differences.append(root)
        for root in acceptable_differences:
            del difference['type_changes'][root]
        if not difference['type_changes']:
            del difference['type_changes']

    return difference

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

        plain_difference = check_plain_difference(original_plain, result)
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
    # Walk through schema for all standard ASN.1 schema files.
    default_schema_list = []
    for root, _, schemafiles in os.walk(os.path.join(
            os.path.dirname(os.path.realpath(__file__)), "schema")):
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
