#include <stdio.h>
#include <ctype.h>

int is_vowel(int charr) {
    charr = tolower((unsigned char)charr);
    return (charr == 'a' || charr == 'e' || charr == 'i' || charr == 'o' || charr == 'u');
}

int main(void) {
    char fname[256];
    printf("Введите имя файла: ");
    if (scanf("%255s", fname) != 1) return 1;

    FILE *F = fopen(fname, "r");
    if (!F) {
        perror("Ошибка открытия файла");
        return 1;
    }

    int charr, total = 0, lines = 0, digits = 0, punct = 0, space = 0;
    int letters = 0, lower = 0, upper = 0, vowels = 0, consonants = 0;

    while ((charr = fgetc(F)) != EOF) {
        total++;
        if (charr == '\n') lines++;
        if (isdigit((unsigned char)charr)) digits++;
        if (ispunct((unsigned char)charr)) punct++;
        if (isspace((unsigned char)charr)) space++;
        if (isalpha((unsigned char)charr)) {
            letters++;
            if (islower((unsigned char)charr)) lower++;
            if (isupper((unsigned char)charr)) upper++;
            if (is_vowel(charr)) vowels++;
            else consonants++;
        }
    }

    fclose(F);

    printf("\nРезультаты n");
    printf("Всего символов: %d\n", total);
    printf("Строк: %d\n", lines);
    printf("Цифр: %d\n", digits);
    printf("Знаков препинания: %d\n", punct);
    printf("Пробельных символов: %d\n", space);
    printf("Букв: %d (строчных: %d, прописных: %d)\n", letters, lower, upper);
    printf("Гласных: %d\n", vowels);
    printf("Согласных: %d\n", consonants);

    return 0;
}