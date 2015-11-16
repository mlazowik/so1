CC = gcc
CFLAGS = -Wall -std=c99 --pedantic -c
LDFLAGS = -Wall -std=c99 --pedantic

OBJECTS = err.o
ALL = ToONP

all: $(ALL)

%.o : %.c
	$(CC) $(CFLAGS) $<

$(ALL) : % : %.o $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o $(ALL) *~
