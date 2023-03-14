CC=gcc
CFLAGS= -O3 -DLIKWID_PERFMON -std=c11 -Wall -Wextra -pedantic -g -I../include -L /usr/bin/lib -I /usr/bin/include
DEBUGFLAGS= -std=c11 -Wall -Wextra -pedantic -g -I../include
LDFLAGS= -llikwid

SRC_C=$(wildcard *.c)
OBJC=$(SRC_C:%.c=%.o)
BIN=a_star

all: $(BIN)

debug: $(SRC_C)
	$(CC) $(DEBUGFLAGS) -o $(BIN) $^ ${LDFLAGS}

$(BIN): $(OBJC)
	$(CC) -o $(BIN) $^ ${LDFLAGS}

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 


clean:
	rm -f *.o
	rm $(BIN)

.PHONY : all
.PHONY : debug
.PHONY : clean
