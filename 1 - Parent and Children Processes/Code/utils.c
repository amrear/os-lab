#include <math.h>
#include <stdbool.h>

#include "utils.h"

// The function that we want to take the integral of.
double f(double x)
{
    return 10 * fabs(sin(0.25 * x));
}

// Calculate the area of a trapezoid.
double trapezoid_area(double a, double b, double h)
{
    return (a + b) * h / 2.0;
}

// Integrate the function f on the interval [a, b] with a specific dx via trapezoidal rule.
double integrate(double a, double b, double f(double x), double dx)
{
    double integral = 0.0;
    for (double i = a; !equalf(i, b, dx * 0.1); i += dx)
        integral += trapezoid_area(f(i), f(i + dx), dx);

    return integral;
}

// Check the equality of two doubles.
bool equalf(double a, double b, double epsilon)
{
    double diff = fabs(a - b);
    return diff < epsilon;
}