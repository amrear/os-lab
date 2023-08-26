#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    double dx, integral = 0.0;

    double partial_integrals[10];

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
        int a = vfork();
        if (a == 0)
        {
            partial_integrals[i] = integrate(i, i + 1, f, dx);

            printf("Area under [%d, %d] was calculated to be %.2lf by process %d. Parent process: %d.\n", i, i + 1, partial_integrals[i], getpid(), getppid());

            exit(i);
        }
    }

    // Retrieve the results from each process and sum them up the get the overall results.
    for (int i = 0; i < 10; i++)
    {
        int tmp;
        wait(&tmp);
        if (WIFEXITED(tmp))
        {
            integral += partial_integrals[WEXITSTATUS(tmp)];
        }
        else
        {
            fprintf(stderr, "Operation failed. One or more process(es) couldn't calculate the integral.\n");
            return EXIT_FAILURE;
        }
    }

    printf("\n");

    printf("Integral of f(x) = 10|sin(0.25x)| on the interval [0, 10] is roughly %.15lf.\n", integral);

    return EXIT_SUCCESS;
}
