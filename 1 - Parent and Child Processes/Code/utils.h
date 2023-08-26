#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

double f(double x);
double trapezoid_area(double a, double b, double h);
double integrate(double a, double b, double f(double x), double dx);
bool equalf(double a, double b, double epsilon);

#endif