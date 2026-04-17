#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PEOPLE 100
#define MAX_NAME 64

typedef struct Person {
    char name[MAX_NAME];
    int year;
    char gender;
    double height;
} Person;

Person data[MAX_PEOPLE]; 
int count = 0;
int sort_keys[8];
int sort_len = 0;

int cmp(const void *a, const void *b) {
    const Person *p1 = (const Person *)a;
    const Person *p2 = (const Person *)b;
    for (int i = 0; i < sort_len; i++) {
        switch (sort_keys[i]) {
            case 1: { int r = strcmp(p1->name,p2->name); if (r) return r; break;}
            case 2: if (p1->year != p2->year) return p1->year - p2->year; break;
            case 3: if (p1->gender != p2->gender) return (unsigned char)p1->gender - (unsigned char)p2->gender; break;
            case 4: if (fabs(p1->height - p2->height) > 1e-9) return (p1->height < p2->height) ? -1 : 1; break;
        }
    }
    return 0;

}

void read_file() {
    FILE *f = fopen("people.txt", "r");
    if (!f) {puts("Файл не создан."); exit(1);}
    char line[256];
    while ((fgets(line, sizeof(line), f) != NULL) && (count < MAX_PEOPLE)){
        line[strcspn(line, "\n\r")] = '\0';
        if (sscanf(line, "%63[^,],%d,%c,%lf", data[count].name, &data[count].year, &data[count].gender, &data[count].height) == 4)
                    count++;
        }
     fclose(f);
};

int main() {
    read_file();

    printf("Поля сортировки (1:имя 2:год 3:пол 4:рост) через пробел: ");
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    char *t = strtok(buf, " \n");
    while (t && sort_len < 8) {
        int v = atoi(t);
        if (v >= 1 && v <= 4) sort_keys[sort_len++] = v;
        t = strtok(NULL, " \n");
    }
    if (!sort_len) { sort_keys[0] = 2; sort_len = 1; }

    qsort(data, count, sizeof(Person), cmp);

    puts("\nРезультат:");
    for (int i = 0; i < count; i++)
        printf("%s | %d | %c | %.2f\n", data[i].name, data[i].year, data[i].gender, data[i].height);

    return 0;

}
