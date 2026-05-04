/*
 * from_translit.c — обратный конвертер: транслит -> русский текст (CP1251).
 *
 * Сборка:  gcc -O2 -o from_translit from_translit.c
 * Запуск:  ./from_translit file1_translit.txt file2_translit.txt ...
 *
 * Для каждого входного файла создаётся <имя>_ru.txt в кодировке CP1251.
 * Алгоритм однопроходный + жадный разбор транслита (длинные сочетания первее).
 * Каждое слово выводится с количеством символов транслита, по правой границе.
 *
 * Формат входного файла (из to_translit):
 *   каждая строка: "<пробелы><транслит_слово>  (N симв.)"
 * Берём первый токен каждой строки — это транслитерированное слово.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { const char *lat; unsigned char ru; } RevEntry;

static const RevEntry REV_TABLE[] = {
    {"Sch", 0xD9}, {"SCH", 0xD9}, {"sch", 0xF9},
    {"Yo",  0xA8}, {"YO",  0xA8}, {"yo",  0xB8},
    {"Zh",  0xC6}, {"ZH",  0xC6}, {"zh",  0xE6},
    {"Kh",  0xD5}, {"KH",  0xD5}, {"kh",  0xF5},
    {"Ts",  0xD6}, {"TS",  0xD6}, {"ts",  0xF6},
    {"Ch",  0xD7}, {"CH",  0xD7}, {"ch",  0xF7},
    {"Sh",  0xD8}, {"SH",  0xD8}, {"sh",  0xF8},
    {"Yu",  0xDE}, {"YU",  0xDE}, {"yu",  0xFE},
    {"Ya",  0xDF}, {"YA",  0xDF}, {"ya",  0xFF},
    {"A",0xC0},{"B",0xC1},{"V",0xC2},{"G",0xC3},{"D",0xC4},
    {"E",0xC5},{"Z",0xC7},{"I",0xC8},{"J",0xC9},{"K",0xCA},
    {"L",0xCB},{"M",0xCC},{"N",0xCD},{"O",0xCE},{"P",0xCF},
    {"R",0xD0},{"S",0xD1},{"T",0xD2},{"U",0xD3},{"F",0xD4},
    {"Y",0xDB},
    {"a",0xE0},{"b",0xE1},{"v",0xE2},{"g",0xE3},{"d",0xE4},
    {"e",0xE5},{"z",0xE7},{"i",0xE8},{"j",0xE9},{"k",0xEA},
    {"l",0xEB},{"m",0xEC},{"n",0xED},{"o",0xEE},{"p",0xEF},
    {"r",0xF0},{"s",0xF1},{"t",0xF2},{"u",0xF3},{"f",0xF4},
    {"y",0xFB},
    {NULL, 0}
};

#define MAX_LINE  4096
#define MAX_RU    512
#define MAX_ROW   (MAX_RU + 32)

static int detransliterate(const char *word, int wlen, unsigned char *buf) {
    int pos = 0, i = 0;
    while (i < wlen) {
        int matched = 0;
        for (int e = 0; REV_TABLE[e].lat != NULL; e++) {
            int klen = (int)strlen(REV_TABLE[e].lat);
            if (i + klen <= wlen &&
                memcmp(word + i, REV_TABLE[e].lat, klen) == 0)
            {
                buf[pos++] = REV_TABLE[e].ru;
                i += klen;
                matched = 1;
                break;
            }
        }
        if (!matched)
            buf[pos++] = (unsigned char)word[i++];
    }
    buf[pos] = '\0';
    return pos;
}

static void process_file(const char *path) {
    FILE *fin = fopen(path, "rb");
    if (!fin) { fprintf(stderr, "[!] cant open file: %s\n", path); return; }

    char    **rows    = NULL;
    int      *rowlens = NULL;
    int       count   = 0, cap = 0, max_col = 0;

    char          line[MAX_LINE];
    unsigned char ru[MAX_RU];
    char          row[MAX_ROW];

    while (fgets(line, sizeof(line), fin)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '\n' || *p == '\r') continue;

        char *tok_start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') p++;
        int wlen = (int)(p - tok_start);
        if (wlen == 0) continue;

        char word[MAX_LINE];
        memcpy(word, tok_start, wlen);
        word[wlen] = '\0';

        int rulen = detransliterate(word, wlen, ru);
        int rlen  = snprintf(row, MAX_ROW, "%.*s  (%d symv.)",
                             rulen, (char *)ru, wlen);
        if (rlen > max_col) max_col = rlen;

        if (count == cap) {
            cap = cap ? cap * 2 : 1024;
            rows    = realloc(rows,    cap * sizeof(char *));
            rowlens = realloc(rowlens, cap * sizeof(int));
            if (!rows || !rowlens) {
                fprintf(stderr, "[!] Out of storage\n");
                fclose(fin); return;
            }
        }
        rows[count]    = strdup(row);
        rowlens[count] = rlen;
        count++;
    }
    fclose(fin);

    if (count == 0) {
        fprintf(stderr, "[!] Empty file: %s\n", path);
        free(rows); free(rowlens); return;
    }

    char outpath[4096];
    const char *dot = strrchr(path, '.');
    const char *sl  = strrchr(path, '/');
    if (!sl) sl = strrchr(path, '\\');
    int base_len = (dot && (!sl || dot > sl)) ? (int)(dot - path) : (int)strlen(path);
    if (base_len >= 9 && memcmp(path + base_len - 9, "_translit", 9) == 0)
        base_len -= 9;
    snprintf(outpath, sizeof(outpath), "%.*s_ru.txt", base_len, path);

    FILE *fout = fopen(outpath, "wb");
    if (!fout) {
        fprintf(stderr, "[!] Cant create: %s\n", outpath);
    } else {
        for (int i = 0; i < count; i++) {
            int pad = max_col - rowlens[i];
            for (int p2 = 0; p2 < pad; p2++) fputc(' ', fout);
            fwrite(rows[i], 1, rowlens[i], fout);
            fputc('\n', fout);
        }
        fclose(fout);
        fprintf(stdout, "[OK] %s -> %s  (%d words)\n", path, outpath, count);
    }

    for (int i = 0; i < count; i++) free(rows[i]);
    free(rows); free(rowlens);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++)
        process_file(argv[i]);
    return 0;
}