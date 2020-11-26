CFLAGS=-Os -Wall -Werror -g
OBJECTS=cs_decode.o test.o schema.o

all:    $(OBJECTS)
	cc -o test $^

clean:
	rm -f *.o