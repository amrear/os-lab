#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    double integral, dx;

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

    // Calculate dx in a sequential manner with only one process.
    integral = integrate(0, 10, f, dx);

    printf("Integral of f(x) = 10|sin(0.25x)| on the interval [0, 10] is roughly %.2lf.\n", integral);

    return EXIT_SUCCESS;
}