NANOPB=$(HOME)/SiFive/nanopb
CFLAGS=-Os -Wall -Werror -g -I$(NANOPB)
RV32_CC=riscv32-unknown-elf-gcc
RV64_CC=riscv64-unknown-elf-gcc

SOURCES=$(wildcard *.c) \
	$(NANOPB)/pb_common.c \
	$(NANOPB)/pb_decode.c
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=test
RV32_OBJECTS=$(patsubst %.o,%-rv32.o,$(OBJECTS))
RV64_OBJECTS=$(patsubst %.o,%-rv64.o,$(OBJECTS))

all:	$(TARGET) $(TARGET)-rv32 $(TARGET)-rv64

size:	$(RV32_OBJECTS) $(RV64_OBJECTS) $(TARGET)-rv32 $(TARGET)-rv64
	echo RV32: && \
	    size -t $(RV32_OBJECTS) && \
	    size $(TARGET)-rv32 && \
	    echo RV64: && \
	    size -t $(RV64_OBJECTS) && \
	    size $(TARGET)-rv64

$(TARGET):	$(OBJECTS)
	$(CC) -DDEBUG $(CFLAGS) -o $@ $^

%-rv32.o:	%.c
	$(RV32_CC) $(CFLAGS) -c -o $@ $^

$(TARGET)-rv32:	$(RV32_OBJECTS)
	$(RV32_CC) -o $@ $^

%-rv64.o:	%.c
	$(RV64_CC) $(CFLAGS) -c -o $@ $^

$(TARGET)-rv64:	$(RV64_OBJECTS)
	$(RV64_CC) -o $@ $^

clean:
	rm -f $(OBJECTS) $(RV32_OBJECTS) $(RV64_OBJECTS)

