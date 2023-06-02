CC=gcc
CFLAGS= -Wall -Wextra -Werror -pedantic -g
OPTFLAGS= -O3 -march=native

TARGETS=mandelbrot fft

all: $(TARGETS)

mandelbrot: mandelbrot.c
	$(CC) $(CFLAGS) $(OPTFLAGS) -o mandelbrot mandelbrot.c -lm

fft: fft.c
	$(CC) $(CFLAGS) -o fft fft.c -lm

clean:
	rm -f $(TARGETS)
	rm -f *.ppm

PHONY: clean