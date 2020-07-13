#!/usr/bin/python3

############################################################
### Schema definition, could be read from JSON/YAML
############################################################
schema = {
    "hartid": {"type": "number", "code": 7},
    "debug": {
        "type": "schema", "code": 9,
        "schema": {
            "abstract": {
                "type": "list", "code": 7,
                "schema": {
                    "tuple": {"type": "triple", "code": 2},
                    "triple": {"type": "triple", "code": 3},
                }
            }
        }
    }
}

############################################################
### Human-readable configuration structure, could be read from JSON/YAML
############################################################
LOW = 12345
HIGH = 0xfffff
MASK = 0xffffa
A = 9675309
B = 0xdeadbeef
configuration = {
    "hartid": 1234,
    "debug": {
        "abstract": [
            {"triple": [LOW, HIGH, MASK]},
            {"tuple": [LOW, HIGH, MASK]}
        ]
    }
}

############################################################
### Source code of software tool
############################################################
simple_encoders = {
    "number": lambda v: "%s" % v,
    "triple": lambda v: "%s,%s,%s" % (v[0], v[1], v[2]),
    "tuple": lambda v: "%s,%s" % (v[0], v[1])
}

def encode_list(schema, value):
    return "".join((encode(schema, entry) for entry in value)) + "EOL"

def encode(schema, configuration):
    result = ""
    for key, value in configuration.items():
        typ = schema[key]["type"]
        if typ in simple_encoders:
            result += "<%d>%s\n" % (schema[key]["code"], simple_encoders[typ](value))
        elif typ == "schema":
            result += "<%d>%sEND\n" % (schema[key]["code"], encode(schema[key]["schema"], value))
        elif typ == "list":
            result += "<%d>%s\n" % (schema[key]["code"], encode_list(schema[key]["schema"], value))
        else:
            assert False, "Unsupported type: %r" % typ
    return result

print(encode(schema, configuration))
