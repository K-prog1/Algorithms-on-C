#include <stdio.h>
#include "maths.h"

int main() {
    int n = 4;
    printf("Sum: %.2f\n", calc_sum(n, 1.5, 2.5, 3.5, 4.5));
    printf("Max: %.2f\n", calc_max(n, 1.5, 2.5, 3.5, 4.5));
    printf("Min: %.2f\n", calc_min(n, 1.5, 2.5, 3.5, 4.5));
    printf("Avg: %.2f\n", calc_avg(n, 1.5, 2.5, 3.5, 4.5));
    return 0;
}