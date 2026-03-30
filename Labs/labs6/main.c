#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE *in = fopen("input.txt", "r" );
    FILE *out = fopen("output.txt", "w");

    char surname[51], name[51], patronym[51];

    int year;

    while(scanf("%s %s %s %d", surname, name, patronym, &year)==4)
    {
        if(year > 1980){
            fprintf(out,"%s %s %s %d", surname, name, patronym, year);
        }


    }
    fclose(in);
    fclose(out);
    return 0;
}