#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

double PI;
int SIZE = 262144;
typedef double complex cplx;

void _fft(cplx buf[], cplx out[], int n, int step)
{
    if (step < n)
    {
        _fft(out, buf, n, step * 2);
        _fft(out + step, buf + step, n, step * 2);

        for (int i = 0; i < n; i += 2 * step)
        {
            cplx t = cexp(-I * PI * i / n) * out[i + step];
            buf[i / 2] = out[i] + t;
            buf[(i + n) / 2] = out[i] - t;
        }
    }
}

void fft(cplx buf[], int n)
{
    cplx out[n];
    for (int i = 0; i < n; i++)
        out[i] = buf[i];

    _fft(buf, out, n, 1);
}

void show(const char *s, cplx buf[])
{
    printf("%s", s);
    for (int i = 0; i < 8; i++)
        if (!cimag(buf[i]))
            printf("%g ", creal(buf[i]));
        else
            printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

int main()
{
    PI = atan2(1, 1) * 4;

    cplx *buf = malloc(SIZE * sizeof(cplx));
    if (buf == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
        buf[i] = rand() & 1;

    show("Data: ", buf);
    fft(buf, SIZE);
    show("\nFFT : ", buf);

    free(buf);
    return 0;
}