#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

int code_of(char c) {
    switch (c) {
        case 'A': return 001;
        case 'C': return 002;
        case 'G': return 004;
        case 'T': return 007;
        case 'N': return 000;
        default: return -1;
    }
}

int main(int argc, char *argv[]) {
    const char *fname = (argc > 1) ? argv[1] : "input.txt";
    FILE *fin = fopen(fname, "r");
    if (!fin) {
        perror("fopen input");
        return 1;
    }

    FILE *fout = fopen("output.bin", "wb");
    if (!fout) {
        perror("fopen output");
        fclose(fin);
        return 1;
    }

    long countA = 0, countG = 0, countC = 0, countT = 0, countN = 0;

    ll buffer = 0;
    int bitsUsed = 0; 
    const int LL_BITS = 63; 

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        if (ch == '\n' || ch == '\r') continue;

        int code;
        switch (ch) {
            case 'A': code = 001; countA++; break;
            case 'G': code = 004; countG++; break;
            case 'C': code = 002; countC++; break;
            case 'T': code = 007; countT++; break;
            case 'N': code = 000; countN++; break;
            default: continue; 
        }

        buffer = (buffer << 3) | code;
        bitsUsed += 3;

        if (bitsUsed == LL_BITS) {
            fwrite(&buffer, sizeof(ll), 1, fout);
            buffer = 0;
            bitsUsed = 0;
        }
    }

    if (bitsUsed > 0) {
        buffer <<= (LL_BITS - bitsUsed); 
        fwrite(&buffer, sizeof(ll), 1, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("Count A: %ld\n", countA);
    printf("Count G: %ld\n", countG);
    printf("Count C: %ld\n", countC);
    printf("Count T: %ld\n", countT);
    printf("Count N: %ld\n", countN);

    return 0;
}