PROTO = $(wildcard */*.proto)
PB2 = $(PROTO:.proto=_pb2.py)

all: $(PB2)

%_pb2.py:	%.proto
	protoc -Ischema $< --python_out=python
