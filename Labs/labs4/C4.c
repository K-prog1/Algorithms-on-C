#include <math.h>
#include "triangle.h"

double calculatePerimetr(double a, double b, double c){
    return a + b + c;
}

double calculatePloshad(double a, double b, double c){
    double p = calculatePerimetr(a, b, c);
    return sqrt(p*(p-a)*(p-b)*(p-c));
}

int ValidTriangle(double a, double b, double c){
    return (a+b>c)&&(a+c>b)&&(b+c>a);
}