BIN = descriptions/example-compact-custom.bin \
    descriptions/example-compact-protobuf.bin \
    descriptions/example-custom.bin \
    descriptions/example-protobuf.bin

all: $(BIN)

descriptions/%-custom.bin:	descriptions/%.yaml
	./custom-encoding/encode.sh $<

descriptions/%-protobuf.bin:	descriptions/%.yaml
	./protobuf/encode.sh $<

clean:
	rm -f $(BIN)
