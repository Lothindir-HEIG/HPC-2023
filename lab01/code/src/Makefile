CC=gcc
CFLAGS= -O3 -std=c11 -Wall -Wextra -pedantic -g -I../include
# CFLAGS= -std=c11 -Wall -Wextra -pedantic -g -I../include
DEBUGFLAGS= -std=c11 -Wall -Wextra -pedantic -g -I../include

SRC_C=$(wildcard *.c)
OBJC=$(SRC_C:%.c=%.o)
BIN=a_star

all: $(BIN)

debug: $(SRC_C)
	$(CC) $(DEBUGFLAGS) -o $(BIN) $^

$(BIN): $(OBJC)
	$(CC) -o $(BIN) $^ -lubsan

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< -lubsan


clean:
	rm -f *.o
	rm $(BIN)

.PHONY : all
.PHONY : debug
.PHONY : clean
