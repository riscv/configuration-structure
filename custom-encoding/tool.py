#!/usr/bin/python3

############################################################
# Schema definition, could be read from JSON/YAML
############################################################
schema = {
    # Config structure group owns this part
    "range": {
        "start": {"required": True, "type": "Number", "code": 1},
        "length": {"required": True, "type": "Number", "code": 2}
    },
    "hart": {
        "harts": {"required": True, "repeatable": True, "type": "range", "code": 1},
        "debug": {"type": "debug", "code": 16}
    },
    "tuple": {
        "value": {"required": True, "type": "Number", "code": 1},
        "mask": {"required": True, "type": "Number", "code": 2}
    },
    "triple": {
        "start": {"required": True, "type": "Number", "code": 1},
        "length": {"required": True, "type": "Number", "code": 2},
        "mask": {"required": True, "type": "Number", "code": 3}
    },
    "possible_values": {
        "tuple": {"repeatable": True, "type": "tuple", "code": 1},
        "triple": {"repeatable": True, "type": "triple", "code": 2},
    },
    "configuration": {
        "hart": {"repeatable": True, "type": "hart", "code": 1}
    },

    # Debug Group owns this part
    "debug_trigger": {
        "index": {"required": True, "repeatable": True, "type": "range", "code": 1},
        "values": {"required": True, "repeatable": True, "type": "possible_values", "code": 2}
    },
    "debug": {
        "trigger": {"repeatable": True, "type": "debug_trigger", "code": 1}
    },
}

############################################################
# Human-readable configuration structure, could be read from JSON/YAML
############################################################
LOW = 12345
HIGH = 0xfffff
MASK = 0xffffa
A = 9675309
B = 0xdeadbeef
configuration = {
    "hart": [
        {
            "harts": [{"start": 0, "length": 1}],
            "debug": {
                "trigger": [{
                    "index": [{"start": 0, "length": 4}],
                    "values": [{
                        "triple": [{"start": LOW, "length": HIGH - LOW, "mask": MASK}]
                    }]
                }]
            }
        },
        {
            "harts": [{"start": 1, "length": 4}]
        },
    ]
}

############################################################
# Source code of software tool
############################################################
def encode_number(v):
    return "%s;" % v

builtin_encoders = {
    "Number": encode_number
}

"""
Return encoded, and a flag that indicates whether the encoded data
self-describes its length.
"""
def encode(schema, typ, value, repeatable=False):
    #print(typ, value, repeatable)

    encoded = ""

    if repeatable:
        encoded += "".join(encode(schema, typ, v)[0] for v in value)
        describes_length = False
    else:
        if typ in builtin_encoders:
            encoded += builtin_encoders[typ](value)
            # Built-in types take care of the length themselves.
            describes_length = True
        elif typ in schema:
            # First, write all required items in code order. Required items don't
            # need to have the code encoded because the reader knows what to
            # expect.
            schema_required = []
            optional_found = 0
            for k, v in schema[typ].items():
                if v.get("required"):
                    schema_required.append((v["code"], k))
                else:
                    optional_found += 1
            schema_required.sort()
            for _, name in schema_required:
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
            describes_length = optional_found == 0
        else:
            assert False, "Unsupported type: %r" % typ

    return encoded, describes_length


print(encode(schema, "configuration", configuration)[0])