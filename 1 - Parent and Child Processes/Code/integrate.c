#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    double dx, integral = 0.0;

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
            fprintf(stderr, "dx must be smaller than 0.\n");
            return EXIT_FAILURE;
        }
    }

    // Generate 10 processes and give each of then an interval to calculate the integral of.
    for (int i = 0; i < 10; i++)
    {
        int a = fork();
        if (a == 0)
        {
            double tmp = integrate(i, i + 1, f, dx);
            printf("Area under [%d, %d] was calculated to be %lf by process %d.\n", i, i + 1, tmp, getpid());
            exit(tmp);
        }
    }

    // Retrieve the results from each process and sum them up the get the overall results.
    for (int i = 0; i < 10; i++)
    {
        int tmp;
        wait(&tmp);
        if (WIFEXITED(tmp))
        {
            tmp = WEXITSTATUS(tmp);
        }
        else
        {
            fprintf(stderr, "Operation failed. One or more process(es) couldn't calculate the integral.\n");
            return EXIT_FAILURE;
        }
        
        integral += tmp;
    }

    printf("\n");

    printf("Integral of f(x) = 10|sin(0.25x)| on the interval [0, 10] is roughly %.2lf.\n", integral);

    return EXIT_SUCCESS;
}