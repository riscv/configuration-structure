#!/usr/bin/python3

import io
import copy
from pprint import pprint
import traceback
import sys
import json5
import argparse
import os

def encode_number(v):
    return ("%s;" % v).encode('utf-8')

def debug(string):
    print(">>> %s%s" % (" " * len(traceback.extract_stack()), string))

def decode_number(stream):
    buf = b""
    count = 0
    while True:
        c = stream.read(1)
        count += 1
        if c.isdigit():
            buf += c
        else:
            break
    value = int(buf)
    #debug("decoded %d (%db)" % (value, count))
    return value, count

builtin_types = {
    "Number": (encode_number, decode_number)
}

def encode_schema_type(schema, typ, value):
    # First, write all required items in code order. Required items don't
    # need to have the code encoded because the reader knows what to
    # expect.
    encoded = b""
    schema_required = []
    for k, v in schema[typ].items():
        if v.get("required"):
            schema_required.append((v["code"], k))
    schema_required.sort()
    for _, name in schema_required:
        assert name in value, "%r required by %r but not in %r" % (name, typ, value)
        val = value[name]
        e, d = encode(schema, schema[typ][name]["type"], val,
                        repeatable=schema[typ][name].get("repeatable"))
        if not d:
            encoded += encode_number(len(e))
        encoded += e

    # Now write the optional entries, which need the code to show the
    # type.
    for name, val in value.items():
        assert name in schema[typ], "Undefined entry %r" % name
        if schema[typ][name].get("required"):
            continue
        encoded += encode_number(schema[typ][name]["code"])
        e, d = encode(schema, schema[typ][name]["type"], val,
                        repeatable=schema[typ][name].get("repeatable"))
        if not d:
            encoded += encode_number(len(e))
        encoded += e
    describes_length = not needs_length(schema[typ])

    return encoded, describes_length

"""
Return encoded, and a flag that indicates whether the encoded data
self-describes its length.
"""
def encode(schema, typ, value, repeatable=False):
    #print(typ, value, repeatable)

    if repeatable:
        encoded = b""
        for v in value:
            e, d = encode(schema, typ, v)
            if not d:
                encoded += encode_number(len(e))
            encoded += e
        describes_length = False
    else:
        if typ in builtin_types:
            encoded = builtin_types[typ][0](value)
            # Built-in types take care of the length themselves.
            describes_length = True
        elif typ in schema:
            encoded, describes_length = encode_schema_type(schema, typ, value)
        else:
            assert False, "Unsupported type: %r" % typ

    return encoded, describes_length

def encode_with_length(schema, typ, value, repeatable=False):
    encoded, describes_length = encode(schema, typ, value, repeatable=repeatable)
    if describes_length:
        return encoded
    else:
        return encode_number(len(encoded)) + encoded

def required_entries(description):
    result = []
    for value in description.values():
        if value.get("required"):
            # This modifies the schema, not a copy. But that's OK.
            result.append(value)
    result.sort(key=lambda v: v["code"])
    return result

def needs_length(description):
    for entry in description.values():
        if not entry.get("required"):
            return True
    return False

def decode_schema_type(schema, typ, stream):
    count = 0
    description = schema[typ]
    tree = {}

    if needs_length(description):
        remaining, c = decode_number(stream)
        count += c
    else:
        remaining = 0

    for entry in required_entries(description):
        t, c = decode(schema, entry["type"], stream,
                      repeatable=entry.get("repeatable"))
        tree[entry["name"]] = t
        count += c
        remaining -= c

    while remaining > 0:
        code, c = decode_number(stream)
        count += c
        remaining -= c
        entry = description[code]
        t, c = decode(schema, entry["type"], stream,
                    repeatable=entry.get("repeatable"))
        tree[entry["name"]] = t
        count += c
        remaining -= c

    if needs_length(description):
        assert(remaining == 0)

    return tree, count

"""
Return tuple of decoded tree, number of bytes written from the stream.
"""
def decode(schema, typ, stream, repeatable=False):
    #debug("decode %s, repeatable=%r" % (typ, repeatable))
    if repeatable:
        length, count = decode_number(stream)
        l = 0
        tree = []
        while l < length:
            t, c = decode_schema_type(schema, typ, stream)
            count += c
            l += c
            tree.append(t)
        assert(l == length)
        return tree, count
    else:
        if typ in builtin_types:
            return builtin_types[typ][1](stream)
        elif typ in schema:
            return decode_schema_type(schema, typ, stream)
        else:
            assert False, "Unsupported type: %r" % typ

"""Index by code instead of name."""
def build_decode_schema(schema):
    decode_schema = {}
    for type_name, description in schema.items():
        decode_schema[type_name] = {}
        for entry_name, entry in description.items():
            entry = copy.copy(entry)
            entry["name"] = entry_name
            decode_schema[type_name][entry["code"]] = entry
    return decode_schema

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--schema', default='schema.json5')
    parser.add_argument('filename')
    args = parser.parse_args()

    schema = json5.load(open(args.schema, "r"))
    decode_schema = build_decode_schema(schema)

    if args.filename.endswith(".json5"):
        tree = json5.load(open(args.filename, "r"))
        encoded = encode_with_length(schema, "configuration", tree)
        sys.stdout.buffer.write(encoded)

        # Check that if we decode the encoded data, we get the original tree
        # back.
        tree2, length = decode(decode_schema, "configuration", io.BytesIO(encoded))

        assert(length == len(encoded))
        assert(tree2 == tree)

    elif args.filename.endswith(".bin"):
        encoded = open(args.filename, "rb").read()
        tree, length = decode(decode_schema, "configuration", io.BytesIO(encoded))
        sys.stdout.write(json5.dumps(tree, indent=2))

sys.exit(main())