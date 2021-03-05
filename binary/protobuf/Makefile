NANOPB = $(HOME)/SiFive/nanopb

PROTO_FILES = $(wildcard schema/*.proto)
PYTHON_FILES = $(patsubst schema/%,python/%,$(PROTO_FILES:.proto=_pb2.py))
CPP_FILES = $(patsubst schema/%,cpp/%,$(PROTO_FILES:.proto=.pb.cc))
NANOPB_FILES = $(patsubst schema/%,nanopb/%,$(PROTO_FILES:.proto=.pb.c))

all: $(PYTHON_FILES) $(CPP_FILES)

nanopb:	$(NANOPB_FILES)

python/%_pb2.py:	schema/%.proto
	protoc -Ischema $< --python_out=python

cpp/%.pb.cc:	schema/%.proto
	protoc -Ischema $< --cpp_out=cpp

nanopb/%.pb.c:	schema/%.proto
	python $(NANOPB)/nanopb_generator.py -Ischema -Dnanopb $<

test.bin: test.pbtxt
	protoc -Ischema --encode=Configuration main.proto <$< >$@
