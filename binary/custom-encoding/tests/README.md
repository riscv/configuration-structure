No automation yet, but here's the intent:

1. Encode description.json5 using schema.json5.
2. Decode the binary using schema-limited.json5.
3. Check that the input and the output match, except that in the output start
   in flexible\_range is missing, and debug\_trigger\_mcontrol is also missing.
