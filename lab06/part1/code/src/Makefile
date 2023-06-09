CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lm

all: non_parallel parallel

non_parallel: non_parallel.o
	$(CC) $(CFLAGS) -o non_parallel non_parallel.o $(LDFLAGS)

non_parallel.o: non_parallel.c
	$(CC) $(CFLAGS) -c non_parallel.c

parallel: parallel.o
	$(CC) $(CFLAGS) -o parallel parallel.o $(LDFLAGS)

parallel.o: parallel.c
	$(CC) $(CFLAGS) -c parallel.c

clean:
	rm -f non_parallel parallel *.o
