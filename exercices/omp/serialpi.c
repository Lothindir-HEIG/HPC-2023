#include <omp.h>

#define NUM_THREADS 4

static long num_steps = 100000;
double step;

int main()
{
    omp_set_num_threads(NUM_THREADS);
    int i;
    double pi, sum[NUM_THREADS];
    step = 1.0 / (double)num_steps;
#pragma omp parallel
    {
        int i, id, nthreads;
    }
}