CFLAGS=-Os -Wall -Werror -g
RV32_CC=riscv32-unknown-elf-gcc
RV64_CC=riscv64-unknown-elf-gcc

OBJECTS=cs_decode.o test.o schema.o
TARGET=test
RV32_OBJECTS=$(patsubst %.o,%-rv32.o,$(OBJECTS))
RV64_OBJECTS=$(patsubst %.o,%-rv64.o,$(OBJECTS))

all:	$(TARGET) $(TARGET)-rv32 $(TARGET)-rv64

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

schema.c:
	../tool.py --schema ../schema.json5 source

clean:
	rm -f *.o schema.c
