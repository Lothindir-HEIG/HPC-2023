CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm

all: non_parallel parallel

non_parallel: non_parallel.o
	$(CC) $(CFLAGS) -O2 -o non_parallel non_parallel.o $(LDFLAGS)

non_parallel.o: non_parallel.c
	$(CC) $(CFLAGS) -O2 -c non_parallel.c

parallel: parallel.o
	$(CC) $(CFLAGS) -O3 -fopenmp -o parallel parallel.o $(LDFLAGS)

parallel.o: parallel.c
	$(CC) $(CFLAGS) -O3 -fopenmp -c parallel.c

clean:
	rm -f non_parallel parallel *.o
