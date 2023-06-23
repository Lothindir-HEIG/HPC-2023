CC=gcc
OPT_FLAGS= -march=native -msse4.1 #-fopt-info -flto -fwhole-program -funroll-loops -finline-functions -march=native
CFLAGS= -O2 $(OPT_FLAGS) -fopenmp -std=c11 -Wall -Wextra -pedantic -g -I../include -L /usr/local/lib -I /usr/local/include #-DLIKWID_PERFMON
DEBUGFLAGS= -std=c11 -Wall -Wextra -pedantic -g -I../include
LDFLAGS= -fopenmp #-llikwid

SRC_C=$(wildcard *.c)
OBJC=$(SRC_C:%.c=%.o)
BIN=a_star

all: $(BIN)

test: test.o circ_buffer.o
	$(CC) -o test $^ ${LDFLAGS}

debug: $(SRC_C)
	$(CC) $(DEBUGFLAGS) -o $(BIN) $^ ${LDFLAGS}

$(BIN): $(OBJC)
	$(CC) -o $(BIN) $^ ${LDFLAGS}

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 


clean:
	rm -f *.o
	rm -f $(BIN)
	rm -f test

.PHONY : all
.PHONY : debug
.PHONY : clean
