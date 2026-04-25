#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    char num[256];
    printf("Enter: ");
    if (!fgets(num, sizeof(num), stdin)) return 1;

    char with_space[512];
    int idx = 0;
    int i;
    for (i = 0; num[i] != '\0'; i++) {
        if (idx >= 510) break;
        if (num[i] == '+' || num[i] == '-') {
            with_space[idx++] = ' ';
            with_space[idx++] = num[i];
            with_space[idx++] = ' ';
        } else if (!isspace((unsigned char)num[i])) {
            with_space[idx++] = num[i];
        }
    }
    with_space[idx] = '\0';

    char *p;
    double res = 0.0;
    char *tok = strtok(with_space, " \n\r");
    if (!tok) return 1;
    res = atof(tok);

    while ((p = strtok(NULL, " \n\r")) != NULL) {
        char *v = strtok(NULL, " \n\r");
        if (!v) break;
        double val = atof(v);
        if (*p == '+') res += val;
        else if (*p == '-') res -= val;
    }

    printf("Result: %g\n", res);
    return 0;
}
