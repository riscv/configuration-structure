This is a very minimal protobuf example. `example.proto` contains the schema.
`example.py` contains the human-written configuration description.

Run `make` to generate python code to convert `example.proto` into Python
code, and then you can run `example.py` to examine its output.

As of right now, the output is:
```
Human-readable:
harts {
  isa {
    xlen64: true
    zfinx: true
  }
}
harts {
  isa {
    xlen64: true
  }
}
harts {
  isa {
    xlen64: true
  }
}
harts {
  isa {
    xlen64: true
  }
}
harts {
  isa {
    xlen64: true
  }
}

Binary (turned to hex for printability):
 0a 07 12 05 10 01 80 02 01 0a 04 12 02 10 01 0a 04
 12 02 10 01 0a 04 12 02 10 01 0a 04 12 02 10 01
```