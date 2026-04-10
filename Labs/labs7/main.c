#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[50];
    char surname[50];
    int birth_year;
} humen;

int cmp(const void *a, const void *b) {
    return ((humen*)a)->birth_year - ((humen*)b)->birth_year;
}

int main() {
    humen arr1[4], arr2[4];

    printf("Введи 4 записи (имя фамилия год):\n");
    for (int i = 0; i < 4; i++) {
        scanf("%49s %49s %d", arr1[i].name, arr1[i].surname, &arr1[i].birth_year);
    }

    memcpy(arr2, arr1, sizeof(arr1));
    qsort(arr2, 4, sizeof(humen), cmp);

    printf("\nОтсортировано по году рождения:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s %s %d\n", arr2[i].name, arr2[i].surname, arr2[i].birth_year);
    }

    return 0;
}