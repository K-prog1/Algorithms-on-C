#include "maths.h"

double calc_sum(int count, ...) {
    va_list args;
    va_start(args, count);
    double res = 0.0;
    for (int i = 0; i < count; i++) res += va_arg(args, double);
    va_end(args);
    return res;
}

double calc_max(int count, ...) {
    va_list args;
    va_start(args, count);
    double res = va_arg(args, double);
    for (int i = 1; i < count; i++) {
        double val = va_arg(args, double);
        if (val > res) res = val;
    }
    va_end(args);
    return res;
}

double calc_min(int count, ...) {
    va_list args;
    va_start(args, count);
    double res = va_arg(args, double);
    for (int i = 1; i < count; i++) {
        double val = va_arg(args, double);
        if (val < res) res = val;
    }
    va_end(args);
    return res;
}

double calc_avg(int count, ...) {
    if (count == 0) return 0.0;
    va_list args;
    va_start(args, count);
    double res = 0.0;
    for (int i = 0; i < count; i++) res += va_arg(args, double);
    va_end(args);
    return res / count;
}