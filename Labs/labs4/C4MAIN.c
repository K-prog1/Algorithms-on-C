#include "triangle.h"
#include <stdio.h>
#include <math.h>

int main(){
    double a,b,c;

    printf("Введите 3 значения для треугольника, который могут существовать \n");

    if (scanf("%lf %lf %lf", &a,&b,&c) != 3){
        printf("Введено не 3 числа \n");
        return 1;
    }

    if (ValidTriangle(a,b,c)) {
        printf("Периметр: %.2f\n", calculatePerimetr(a,b,c));
        printf("Площадь: %.2f\n", calculatePloshad(a,b,c));
    }

    return 0;
}