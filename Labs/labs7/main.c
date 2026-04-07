#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
struct humen{
    char* name[24], surname[24];
    int* date;
};

void main(){
    
    FILE *in = fopen("input.txt", "r");
    FILE *out= fopen("output.txt", "w");
    if (!in || !out) { perror("файлы не открыты"); return 1; }

    struct humen source[4];
    struct humen sorted[4];

    

    fclose(in);
    fclose(out);
    return 0;
};