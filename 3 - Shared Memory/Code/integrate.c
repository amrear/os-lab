#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <semaphore.h>
#include <sys/mman.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    double dx, integral = 0.0;

    // Create shared memory for integral calculated by processes as well as two semaphores.
    double *partial_integral = mmap(NULL, sizeof(double), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *s_children = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *s_parent = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(s_children, 1, 1);
    sem_init(s_parent, 1, 0);

    // Get dx from user by means of command line arguments.
    if (argc != 2)
    {
        fprintf(stderr, "    Usage: integral dx\n");
        fprintf(stderr, "      dx: differential of x designated by a real number.\n");
        return EXIT_FAILURE;
    }
    else
    {
        dx = atof(argv[1]);

        if (dx > 1.0)
        {
            fprintf(stderr, "dx must be smaller than 1.\n");
            return EXIT_FAILURE;
        }
    }

    // Generate 10 processes and give each of then an interval to calculate the integral of.
    for (int i = 0; i < 10; i++)
    {
        int a = fork();
        if (a == 0)
        {
            // Calculate the integral related to each process.
            // Heed the mutual exclusion property when writing to the shared memory.
            double tmp = integrate(i, i + 1, f, dx);
            sem_wait(s_children);
            *partial_integral = tmp;
            sem_post(s_parent);
            printf("Area under [%d, %d] was calculated to be %.2lf by process %d. Parent process: %d.\n", i, i + 1, *partial_integral, getpid(), getppid());

            exit(1);
        }
    }

    // Retrieve the results from each process and sum them up the get the overall results.
    // Make sure that its not being written by any other process.
    for (int i = 0; i < 10; i++)
    {
        int tmp;
        wait(&tmp);
        if (WIFEXITED(tmp))
        {
            sem_wait(s_parent);
            integral += *partial_integral;
            sem_post(s_children);
        }
        else
        {
            fprintf(stderr, "Operation failed. One or more process(es) couldn't calculate the integral.\n");
            return EXIT_FAILURE;
        }
    }

    printf("\n");

    printf("Integral of f(x) = 10|sin(0.25x)| on the interval [0, 10] is roughly %.15lf.\n", integral);

    sem_close(s_children);
    sem_close(s_parent);

    munmap(partial_integral, sizeof(double));
    munmap(s_children, sizeof(double));
    munmap(s_parent, sizeof(double));

    return EXIT_SUCCESS;
}
