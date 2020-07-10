#!/usr/bin/python3

import example_pb2
import sys

###
# These lines define the configuration.
###
configuration = example_pb2.Configuration()
hart = configuration.harts.add()
hart.isa.xlen64 = True
hart.isa.zfinx = True
for _ in range(4):
    hart = configuration.harts.add()
    hart.isa.xlen64 = True

###
# This code is just so you can see what we defined.
###

print("Human-readable:")
print(configuration)

print("Binary (turned to hex for printability):")
binary = configuration.SerializeToString()
for i, c in enumerate(binary):
    sys.stdout.write(" ")
    sys.stdout.write("%02x" % c)
    if (i > 0 and (i % 16) == 0):
        sys.stdout.write("\n")
sys.stdout.write("\n")