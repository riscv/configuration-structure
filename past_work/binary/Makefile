SRC = $(wildcard descriptions/*.yaml)
BIN = $(SRC:.yaml=-custom.bin) $(SRC:.yaml=-protobuf.bin)

all: $(BIN)

descriptions/%-custom.bin:	descriptions/%.yaml
	./custom-encoding/encode.sh $<

descriptions/%-protobuf.bin:	descriptions/%.yaml
	./protobuf/encode.sh $<

clean:
	rm -f $(BIN)
