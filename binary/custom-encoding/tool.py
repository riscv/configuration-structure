#!/usr/bin/python3

import copy
from pprint import pprint
import traceback
import sys
import json5
import argparse
import os
import yaml
from bitstring import Bits, BitArray

"""Turn bytes into a stream of bits."""
class BitStream(object):
    def __init__(self, bytes):
        self.bits = Bits(bytes)
        self.offset = 0
    
    def read(self, bitcount):
        result = self.bits[self.offset:self.offset + bitcount]
        self.offset += bitcount
        return result

def debug(string):
    sys.stderr.write(">>> %s%s\n" % (" " * len(traceback.extract_stack()), string))

def error(string):
    sys.stderr.write("%s\n" % string)

compact_encoding = True
number_bits = (3, 4, 5, 7, 10, 14, 19, 25, 32, 40)
assert sum(number_bits) >= 128
def encode_number(v, fixed=None):
    v = v or 0
    if compact_encoding:
        if fixed:
            return Bits(uint=v, length=fixed)

        if v == 0:
            result = Bits(uint=0, length=number_bits[0] + 1)
        else:
            result = BitArray()

            value = v
            for length in number_bits:
                mask = (1<<length) - 1
                result.append(Bits(uint=value & mask, length=length))
                value = value >> length
                result.append(Bits(bool=value>0))
                if value == 0:
                    break
    else:
        result = ("%s;" % v).encode('utf-8')
    #debug("encode %d (0x%x) -> %r" % (v, v, result))
    return result

def decode_number(stream, fixed=None):
    count = 0
    if compact_encoding:
        if fixed:
            value = stream.read(fixed).uint
            count = fixed
        else:
            value = 0
            offset = 0
            for length in number_bits:
                data = stream.read(length).uint
                value = value | (data << offset)
                offset += length
                more = stream.read(1).bool
                count += length + 1
                if more == 0:
                    break
    else:
        buf = b""
        while True:
            c = stream.read(1)
            count += 1
            if c.isdigit():
                buf += c
            else:
                break
        value = int(buf)
    debug("decoded %d (0x%x) in %d bits" % (value, value, count))
    return value, count

def normalize_number(v):
    return int(v)

def encode_boolean(v):
    return encode_number(int(v or 0), fixed=1)

def decode_boolean(stream):
    value, count = decode_number(stream, fixed=1)
    return bool(value), count

def normalize_boolean(v):
    return bool(v)

# TODO: Use classes
builtin_types = {
    "Number": (encode_number, decode_number, normalize_number),
    "Boolean": (encode_boolean, decode_boolean, normalize_boolean)
}

def encode_size(data):
    # BitArray/Bits return size in bits when len() is used.
    return encode_number(len(data))

def encode_schema_type(schema, typ, value):
    #debug("Encoding %r into %r" % (value, typ))
    # First, write all required items in code order. Required items don't
    # need to have the code encoded because the reader knows what to
    # expect.
    encoded = BitArray()
    schema_required = []
    for k, v in schema[typ].items():
        if v.get("required"):
            schema_required.append((v["code"], k))
    schema_required.sort()
    for _, name in schema_required:
        #assert name in value, "%r required by %r but not in %r" % (name, typ, value)
        val = value.get(name)
        e, d = encode(schema, schema[typ][name]["type"], val,
                        repeatable=schema[typ][name].get("repeatable"))
        if not d:
            encoded += encode_size(e)
        encoded += e

    # Now write the optional entries, which need the code to show the
    # type.

    if isinstance(value, list):
        # A list of strings gets converted into a dictionary mapping those
        # strings to true. This lets users specify [A,B] instead of
        # {A: true, B: true}
        value = {k: True for k in value}
    for name, val in value.items():
        assert name in schema[typ], "Undefined entry %r in %r" % (name, typ)
        if schema[typ][name].get("required"):
            continue
        encoded += encode_number(schema[typ][name]["code"])
        e, d = encode(schema, schema[typ][name]["type"], val,
                        repeatable=schema[typ][name].get("repeatable"))
        if not d:
            encoded += encode_size(e)
        encoded += e
    describes_length = not needs_length(schema[typ])

    return encoded, describes_length

def normalize(schema, typ, value, repeatable=False):
    #debug("normalize(%r, %r)" % (typ, value))

    if repeatable:
        if not isinstance(value, list):
            # Don't require an explicit list if there's only one item.
            value = [value]
        lst = []
        for val in value:
            lst.append(normalize(schema, typ, val,
                                 repeatable=False))
        return lst

    if typ in builtin_types:
        return builtin_types[typ][2](value)

    if isinstance(value, list):
        # A list of strings gets converted into a dictionary mapping those
        # strings to true. This lets users specify [A,B] instead of
        # {A: true, B: true}
        value = {k: True for k in value}
    tree = {}

    for name, val in value.items():
        tree[name] = normalize(schema, schema[typ][name]["type"], val,
                               repeatable=schema[typ][name].get("repeatable"))

    # Add default values for every required entry.
    for name, v in schema[typ].items():
        if v.get("required") and name not in tree:
            tree[name] = normalize(schema, schema[typ][name]["type"], None)

    return tree

"""
Return encoded, and a flag that indicates whether the encoded data
self-describes its length.
"""
def encode(schema, typ, value, repeatable=False):
    #error(typ, value, repeatable)

    if repeatable:
        encoded = BitArray()
        if not isinstance(value, list):
            # Don't require an explicit list if there's only one item.
            value = [value]
        for v in value:
            e, d = encode(schema, typ, v)
            if not d:
                encoded += encode_size(e)
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
        return encode_size(encoded) + encoded

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
    debug("decode %s, repeatable=%r" % (typ, repeatable))
    if repeatable:
        length, count = decode_number(stream)
        l = 0
        tree = []
        while l < length:
            if typ in builtin_types:
                t, c = builtin_types[typ][1](stream)
            elif typ in schema:
                t, c = decode_schema_type(schema, typ, stream)
            else:
                assert False, "Unsupported type: %r" % typ
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

def schema_valid(schema):
    result = True
    for typ, entries in schema.items():
        used_codes = set()
        for name, entry in entries.items():
            for key in ("code", "type"):
                if key not in entry:
                    error("No %s defined for %s:%s" % (key, typ, name))
                    result = False
            if entry["code"] in used_codes:
                error("Code for %s:%s already used elsewhere in %s" % (typ, name, typ))
                result = False
            used_codes.add(entry["code"])
    return result

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--schema', default='schema.json5')
    parser.add_argument('--decode', '-d', action='store_true')
    parser.add_argument('--stdout', '-c', action='store_true')
    #TODO: parser.add_argument('--compact-number', '-C', action='store_true')
    parser.add_argument('filename')
    args = parser.parse_args()

    try:
        schema = json5.load(open(args.schema, "r"))
    except ValueError as e:
        error("Failed to load schema from %r:" % args.schema)
        error(e)
        return 1

    if not schema_valid(schema):
        return 1

    decode_schema = build_decode_schema(schema)

    if args.decode:
        encoded = open(args.filename, "rb").read()
        tree, length = decode(decode_schema, "configuration", BitStream(encoded))
        decoded = json5.dumps(tree, indent=2)
        if args.stdout:
            sys.stdout.write(decoded)
        else:
            open(os.path.splitext(args.filename)[0] + ".json5", "w").write(decoded)

    else:
        if args.filename.endswith(".json5"):
            tree = json5.load(open(args.filename, "r"))
        elif args.filename.endswith(".yaml"):
            tree = yaml.load(open(args.filename, "r"), Loader=yaml.FullLoader)
        else:
            raise Exception("Unsupported suffix on %r" % args.filename)
        bitarray = encode_with_length(schema, "configuration", tree)
        encoded = bitarray.tobytes()
        if args.stdout:
            sys.stdout.buffer.write(encoded)
        else:
            open(os.path.splitext(args.filename)[0] + ".bin", "wb").write(encoded)

        # Check that if we decode the encoded data, we get the original tree
        # back.
        tree2, length = decode(decode_schema, "configuration", BitStream(encoded))

        if length != len(bitarray):
            error("Encoded size (%d bits) does not match decoded size (%d bits)!" % (
                len(bitarray), length))
            return 1
        normal_tree = normalize(schema, "configuration", tree)
        normal_tree2 = normalize(schema, "configuration", tree2)
        if (normal_tree2 != normal_tree):
            error("Decoding the encoded tree led to a different result!")
            import deepdiff
            pprint(deepdiff.DeepDiff(normal_tree, normal_tree2), stream=sys.stderr)
            return 1

sys.exit(main())
