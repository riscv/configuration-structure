CFLAGS=-Os -Wall -Werror -g
OBJECTS=cs_decode.o test.o source.o

all:    $(OBJECTS)
	cc -o test $^

clean:
	rm -f *.o