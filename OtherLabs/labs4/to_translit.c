/*
 * to_translit.c — конвертер русского текста (CP1251) в транслит.
 *
 * Сборка:  gcc -O2 -o to_translit to_translit.c
 * Запуск:  ./to_translit file1.txt file2.txt ...
 *
 * Для каждого входного файла создаётся <имя>_translit.txt.
 * Алгоритм однопроходный: файл читается один раз.
 * Каждое слово выводится с количеством символов, выровненным по правой границе.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




static const char *TRANSLIT[256] = {

    [0xA8] = "Yo", 
    [0xB8] = "yo",

    [0xC0]="A",[0xC1]="B",[0xC2]="V",[0xC3]="G",[0xC4]="D",
    [0xC5]="E",[0xC6]="Zh",[0xC7]="Z",[0xC8]="I",[0xC9]="J",
    [0xCA]="K",[0xCB]="L",[0xCC]="M",[0xCD]="N",[0xCE]="O",
    [0xCF]="P",[0xD0]="R",[0xD1]="S",[0xD2]="T",[0xD3]="U",
    [0xD4]="F",[0xD5]="Kh",[0xD6]="Ts",[0xD7]="Ch",[0xD8]="Sh",
    [0xD9]="Sch",[0xDA]="",[0xDB]="Y",[0xDC]="",[0xDD]="E",
    [0xDE]="Yu",[0xDF]="Ya",

    [0xE0]="a",[0xE1]="b",[0xE2]="v",[0xE3]="g",[0xE4]="d",
    [0xE5]="e",[0xE6]="zh",[0xE7]="z",[0xE8]="i",[0xE9]="j",
    [0xEA]="k",[0xEB]="l",[0xEC]="m",[0xED]="n",[0xEE]="o",
    [0xEF]="p",[0xF0]="r",[0xF1]="s",[0xF2]="t",[0xF3]="u",
    [0xF4]="f",[0xF5]="kh",[0xF6]="ts",[0xF7]="ch",[0xF8]="sh",
    [0xF9]="sch",[0xFA]="",[0xFB]="y",[0xFC]="",[0xFD]="e",
    [0xFE]="yu",[0xFF]="ya",
};

#define MAX_WORD   512
#define MAX_TR     (MAX_WORD * 3)   
#define MAX_ROW    (MAX_TR + 32)    
#define MAX_ROWS   (1 << 20)        


static int is_sep(unsigned char b) {

    if (b >= 0xA8) return 0;
    return isspace(b) || ispunct(b);
}

static int transliterate(const unsigned char *word, int wlen, char *buf) {
    int pos = 0;
    for (int i = 0; i < wlen; i++) {
        unsigned char c = word[i];
        const char *rep = TRANSLIT[c];
        if (rep != NULL) {
            int rlen = (int)strlen(rep);
            memcpy(buf + pos, rep, rlen);
            pos += rlen;
        } else {
            buf[pos++] = (char)c;
        }
    }
    buf[pos] = '\0';
    return pos;
}

static void process_file(const char *path) {
    FILE *fin = fopen(path, "rb");
    if (!fin) {
        fprintf(stderr, "[!] Cannot open: %s\n", path);
        return;
    }


    char  **rows    = NULL;
    int    *rowlens = NULL;
    int     count   = 0;
    int     cap     = 0;
    int     max_col = 0;   

    unsigned char word[MAX_WORD];
    int           wlen = 0;
    char          tr[MAX_TR];
    char          row[MAX_ROW];

    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        unsigned char b = (unsigned char)byte;
        if (!is_sep(b)) {
            if (wlen < MAX_WORD - 1)
                word[wlen++] = b;
        } else {
            if (wlen > 0) {
                int trlen = transliterate(word, wlen, tr);
                int rlen = snprintf(row, MAX_ROW, "%s  (%d simvols.)", tr, wlen);
                if (rlen > max_col) max_col = rlen;

                if (count == cap) {
                    cap = cap ? cap * 2 : 1024;
                    rows    = realloc(rows,    cap * sizeof(char *));
                    rowlens = realloc(rowlens, cap * sizeof(int));
                    if (!rows || !rowlens) {
                        fprintf(stderr, "[!] Out of storage\n");
                        fclose(fin);
                        return;
                    }
                }
                rows[count]    = strdup(row);
                rowlens[count] = rlen;
                count++;
                wlen = 0;
            }
        }
    }
    if (wlen > 0) {
        int trlen = transliterate(word, wlen, tr);
        int rlen  = snprintf(row, MAX_ROW, "%s  (%d simvols.)", tr, wlen);
        if (rlen > max_col) max_col = rlen;
        if (count == cap) {
            cap = cap ? cap * 2 : 1024;
            rows    = realloc(rows,    cap * sizeof(char *));
            rowlens = realloc(rowlens, cap * sizeof(int));
        }
        rows[count]    = strdup(row);
        rowlens[count] = rlen;
        count++;
    }
    fclose(fin);

    if (count == 0) {
        fprintf(stderr, "[!] File empty %s\n", path);
        free(rows);
        free(rowlens);
        return;
    }

    char outpath[4096];
  
    const char *dot = strrchr(path, '.');
    const char *sep = strrchr(path, '/');
    if (!sep) sep = strrchr(path, '\\');

    if (dot && (!sep || dot > sep)) {
        int base_len = (int)(dot - path);
        snprintf(outpath, sizeof(outpath), "%.*s_translit.txt", base_len, path);
    } else {
        snprintf(outpath, sizeof(outpath), "%s_translit.txt", path);
    }

    FILE *fout = fopen(outpath, "wb");
    if (!fout) {
        fprintf(stderr, "[!] Cant create file: %s\n", outpath);
    } else {
        for (int i = 0; i < count; i++) {
            int pad = max_col - rowlens[i];
            for (int p = 0; p < pad; p++) fputc(' ', fout);
            fputs(rows[i], fout);
            fputc('\n', fout);
        }
        fclose(fout);
        fprintf(stdout, "[OK] %s -> %s  (%d words)\n", path, outpath, count);
    }

    for (int i = 0; i < count; i++) free(rows[i]);
    free(rows);
    free(rowlens);
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