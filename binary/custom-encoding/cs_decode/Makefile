CFLAGS=-Os -Wall -Werror -g
OBJECTS=cs_decode.o test.o schema.o

all:    $(OBJECTS)
	cc -o test $^

schema.c:
	../tool.py --schema ../schema.json5 source

clean:
	rm -f *.o schema.c
