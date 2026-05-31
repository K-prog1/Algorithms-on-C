#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Макрос вычисляет максимум. Использует statement expression (GCC/Clang) */
#define FIND_MAX(arr, n) ({ \
    int _max = (arr)[0]; \
    for (int _i = 1; _i < (n); _i++) \
        if ((arr)[_i] > _max) _max = (arr)[_i]; \
    _max; \
})

int main(void) {
    char buf[1024];
    int arr[100];
    int count = 0;

    if (!fgets(buf, sizeof(buf), stdin)) return 1;

    char *tok = strtok(buf, " \n");
    while (tok && count < 100) {
        arr[count++] = atoi(tok);
        tok = strtok(NULL, " \n");
    }

    if (count == 0) return 1;
    printf("Максимум: %d\n", FIND_MAX(arr, count));
    return 0;
}