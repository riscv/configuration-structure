#!/usr/bin/python3
import argparse
from argparse import RawTextHelpFormatter
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
        asn1 = asn1tools.compile_files(schema_list, format)
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
        asn1 = asn1tools.compile_files(schema_list, format)
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

def schema_path_dict(schemas_list):
    schema_dict = {}
    for schema in schemas_list:
        path_elements = schema.split (os.path.sep)
        if path_elements[-1].endswith(".asn"):
            schema_dict[path_elements [-1]] = schema
    return schema_dict

def default_schema_dict():
    # Walk through ../schema for all standard ASN.1 schema files.
    default_schema_list = []
    for root, dirs, schemafiles in os.walk(os.path.join (
        os.path.dirname(os.path.realpath(__file__)), "..", "schema")):
        for schemafile in schemafiles:
             default_schema_list.append(root + os.path.sep + schemafile)
    return schema_path_dict(default_schema_list)

def additional_schema_dict(arg_schema):
    additional_schema_list = arg_schema.split (',')
    verified_list = []
    for index, additional_schema in enumerate(additional_schema_list):
        # Validate the schema file.
        schema_file = os.path.normpath(os.path.join (os.path.dirname(os.path.realpath(__file__)), additional_schema))
        if os.path.exists(schema_file):
            verified_list.append(schema_file)
        else:
            print("Schema file: \"" + schema_file + "\" is not exist.")
    return schema_path_dict(verified_list)

schema_help_String  = 'Specify the additional ASN.1 schema files to the standard\n' +\
                      'ones under /schema directory. Multiple ASN.1 schema files\n' +\
                      'could be specified using \',\' as the separator.\n' +\
                      'Example: --schema ../myschema1.asn,../myschema2.asn or\n'+\
                      '         --schema absolute_dir/myschema3.asn\n'
convert_help_String = 'Convert between binary and human-readable formats.'
test_help_String    = 'Test encoding/decoding the given files.'

schema_list = []
def main():
    
    # Build up the default schema dictionary.
    try:
        default_schema = default_schema_dict()
        if len (default_schema) == 0:
            print("No default schemas found.")
            sys.exit ()
    except:
        print("Can't build up the default schema dictionary.")
        sys.exit ()

    parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument('--schema',
            help=schema_help_String)

    subparsers = parser.add_subparsers()

    parse_convert = subparsers.add_parser('convert',
            help=convert_help_String)
    parse_convert.add_argument('source')
    parse_convert.add_argument('destination')
    parse_convert.set_defaults(func=cmd_convert)

    parse_test = subparsers.add_parser('test',
            help=test_help_String)
    parse_test.add_argument('path', nargs='+')
    parse_test.set_defaults(func=cmd_test)

    args = parser.parse_args()

    if args.schema:
        # Build up the dictionary of schemas assigned against to --schema option.
        schema_dictionary = {}
        try:
            # Generate the dictionary of additional schemas.
            schema_dictionary = additional_schema_dict(args.schema)
            if len(schema_dictionary) == 0:
                print("Can't find the schemas assigned to --schema option.")
                return 1
            else:
                # Combine schema_dictionary with the default schema.
                for default_schema_name in default_schema:
                    if default_schema_name not in schema_dictionary:
                        schema_dictionary[default_schema_name] = default_schema[default_schema_name]
        except:
            print("Fail to build up the dictionary of schemas assigned to --schema option.")
            return 1
    else:
        schema_dictionary = default_schema

    # Convert schema dictionary to a list.
    for schema in schema_dictionary:
        schema_list.append (schema_dictionary[schema])

    return args.func(args)

if __name__ == '__main__':
    sys.exit(main())
