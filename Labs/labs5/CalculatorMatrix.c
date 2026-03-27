#include "C5.h"
#include <stdlib.h>

double* calculate(double* A, double* B, char op, int n) {
    double* res = malloc(n * n * sizeof(double));
    if (!res) return NULL;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int idx = i * n + j;
            if (op == '+') res[idx] = A[idx] + B[idx];
            else if (op == '-') res[idx] = A[idx] - B[idx];
            else if (op == '*') {
                res[idx] = 0;
                for (int k = 0; k < n; k++)
                    res[idx] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    return res;
}