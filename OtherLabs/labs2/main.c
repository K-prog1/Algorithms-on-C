#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct word {
    int count;
    char *w;
} WORD;

WORD *words[100000];
int totalw = 0;

int cmpword_alpha(const void *a, const void *b) {
    WORD *w1 = *(WORD **)a;
    WORD *w2 = *(WORD **)b;
    return strcmp(w1->w, w2->w);
}

int cmpword_quant(const void *a, const void *b) {
    WORD *w1 = *(WORD **)a;
    WORD *w2 = *(WORD **)b;
    if (w2->count != w1->count) return w2->count - w1->count;
    return strcmp(w1->w, w2->w);
}

int is_valid_word(const char *s) {
    if (!s || !isalpha((unsigned char)s[0])) return 0;
    for (int i = 1; s[i]; i++)
        if (!isalnum((unsigned char)s[i])) return 0;
    return 1;
}

char *to_lower_dup(const char *s) {
    size_t len = strlen(s);
    char *res = malloc(len + 1);
    if (!res) return NULL;
    for (size_t i = 0; i < len; i++) res[i] = tolower((unsigned char)s[i]);
    res[len] = '\0';
    return res;
}

int find_word(const char *s) {
    for (int i = 0; i < totalw; i++)
        if (strcmp(words[i]->w, s) == 0) return i;
    return -1;
}

void add_word(const char *token) {
    if (!is_valid_word(token)) return;
    char *lower = to_lower_dup(token);
    if (!lower) return;
    
    int idx = find_word(lower);
    if (idx >= 0) {
        words[idx]->count++;
        free(lower);
    } else if (totalw < 100000) {
        WORD *nw = malloc(sizeof(WORD));
        if (!nw) { free(lower); return; }
        nw->w = lower;
        nw->count = 1;
        words[totalw++] = nw;
    } else {
        free(lower);
    }
}

int main(int ac, char *av[]) {
    if (ac < 2) {
        printf("Вы забыли указать список файлов!\n");
        return 1;
    }

    FILE *OUT1 = fopen("dict_alpha.txt", "w");
    FILE *OUT2 = fopen("dict_freq.txt", "w");
    if (!OUT1 || !OUT2) return 1;

    char buf[4096];
    const char *delim = " \t\n\r.,;:!?\"'()[]{}<>-+/*=&%$#@^~|\\";

    for (int i = 1; i < ac; i++) {
        char path[512];
        snprintf(path, sizeof(path), "texts/%s", av[i]);
        FILE *IN = fopen(path, "r");
        if (!IN) continue;
        
        while (fgets(buf, sizeof(buf), IN)) {
            char *tok = strtok(buf, delim);
            while (tok) {
                add_word(tok);
                tok = strtok(NULL, delim);
            }
        }
        fclose(IN);
    }

    qsort(words, totalw, sizeof(WORD *), cmpword_alpha);
    for (int i = 0; i < totalw; i++)
        fprintf(OUT1, "%s %d\n", words[i]->w, words[i]->count);
    
    qsort(words, totalw, sizeof(WORD *), cmpword_quant);
    for (int i = 0; i < totalw; i++)
        fprintf(OUT2, "%s %d\n", words[i]->w, words[i]->count);

    fclose(OUT1);
    fclose(OUT2);
    
    for (int i = 0; i < totalw; i++) {
        free(words[i]->w);
        free(words[i]);
    }
    return 0;
}