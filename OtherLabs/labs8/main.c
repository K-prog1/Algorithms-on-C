/* filter.c
 * Программа-фильтр «сглаживатель»: читает поток чисел (double) из stdin,
 * сглаживает их сверткой со сглаживающими коэффициентами (строка
 * треугольника Паскаля, нормированная на сумму) и выводит результат в stdout.
 *
 * Использование:
 *   ./gen | ./filter [N] | ./osc
 * где N - номер строки треугольника Паскаля (порядок фильтра, по умолчанию 4,
 * т.е. используются коэффициенты 1 4 6 4 1, нормированные на 16).
 *
 * Коэффициенты также можно взять из файла PASCAL.txt, если он есть рядом
 * с программой: тогда нужно передать имя файла вторым параметром.
 *
 * Работает в потоковом режиме (line-buffered), чтобы данные сразу шли
 * дальше по конвейеру в osc, а не копились до конца ввода.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOEF 64

/* Вычисление биномиальных коэффициентов C(n,k) для строки n треугольника Паскаля */
static int pascal_row(int n, double *coef) {
    long long c = 1;
    double sum = 0.0;
    for (int k = 0; k <= n; k++) {
        coef[k] = (double)c;
        sum += coef[k];
        /* следующий биномиальный коэффициент: C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        c = c * (n - k) / (k + 1);
    }
    /* нормировка, чтобы сумма коэффициентов была равна 1 (не менялась амплитуда) */
    for (int k = 0; k <= n; k++)
        coef[k] /= sum;
    return n + 1;
}

/* Попытка прочитать коэффициенты сглаживания из файла PASCAL.txt.
 * Формат: в файле несколько строк, в каждой строке - последовательность
 * целых чисел (одна строка треугольника Паскаля), разделенных пробелами.
 * row - номер строки (начиная с 0), которую нужно использовать. */
static int read_pascal_from_file(const char *fname, int row, double *coef) {
    FILE *f = fopen(fname, "r");
    if (!f) return 0;

    char line[1024];
    int cur = 0;
    int found = 0;
    int ncoef = 0;

    while (fgets(line, sizeof(line), f)) {
        if (cur == row) {
            /* парсим числа из строки */
            char *p = line;
            double sum = 0.0;
            ncoef = 0;
            while (*p) {
                while (*p == ' ' || *p == '\t') p++;
                if (*p == '\0' || *p == '\n') break;
                double val = strtod(p, &p);
                if (ncoef < MAXCOEF) {
                    coef[ncoef++] = val;
                    sum += val;
                }
            }
            if (ncoef > 0 && sum != 0.0) {
                for (int i = 0; i < ncoef; i++)
                    coef[i] /= sum;
                found = 1;
            }
            break;
        }
        cur++;
    }
    fclose(f);
    return found ? ncoef : 0;
}

int main(int argc, char *argv[]) {
    double coef[MAXCOEF];
    int ncoef;

    int order = 4;           /* строка треугольника Паскаля по умолчанию */
    const char *pfile = NULL;

    if (argc > 1) order = atoi(argv[1]);
    if (argc > 2) pfile = argv[2];

    if (pfile != NULL) {
        ncoef = read_pascal_from_file(pfile, order, coef);
        if (ncoef == 0) {
            fprintf(stderr, "filter: не удалось прочитать коэффициенты из %s, "
                             "использую встроенный треугольник Паскаля (строка %d)\n",
                    pfile, order);
            ncoef = pascal_row(order, coef);
        }
    } else {
        ncoef = pascal_row(order, coef);
    }

    fprintf(stderr, "filter: используется окно из %d коэффициентов:", ncoef);
    for (int i = 0; i < ncoef; i++)
        fprintf(stderr, " %.5f", coef[i]);
    fprintf(stderr, "\n");

    /* Кольцевой буфер последних ncoef значений */
    double *buf = calloc(ncoef, sizeof(double));
    int filled = 0;   /* сколько значений реально накоплено (для старта) */
    int pos = 0;      /* позиция для следующей записи в кольцевом буфере */

    double x;
    /* Делаем stdout построчно небуферизованным, чтобы данные сразу
     * уходили дальше по конвейеру в osc (важно для потокового режима). */
    setvbuf(stdout, NULL, _IOLBF, 0);

    while (scanf("%lf", &x) == 1) {
        buf[pos] = x;
        pos = (pos + 1) % ncoef;
        if (filled < ncoef) filled++;

        /* Пока буфер не заполнен полностью, сворачиваем с тем,
         * что уже накоплено (используя доступные последние значения,
         * дублируя первый элемент для недостающих отсчетов -
         * простая защита от краевого эффекта в начале потока). */
        double acc = 0.0;
        double wsum = 0.0;
        for (int k = 0; k < ncoef; k++) {
            int idx;
            if (filled < ncoef) {
                /* буфер еще не заполнен - берем только уже имеющиеся значения */
                if (k >= filled) continue;
                idx = k;
            } else {
                idx = (pos + k) % ncoef; /* от самого старого к самому новому */
            }
            acc += coef[k] * buf[idx];
            wsum += coef[k];
        }
        double y = (wsum > 0.0) ? acc / wsum : x;

        printf("%f\n", y);
    }

    free(buf);
    return 0;
}