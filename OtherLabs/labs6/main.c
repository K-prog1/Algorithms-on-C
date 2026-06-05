#include "hull.h"

int main(int argc, char *argv[]) {
    FILE *In, *Out;
    int i = 0;
    int num; 
    double x, y;
    In = fopen(argv[1], "r");
    Out = fopen("OUT.dat", "w");
    while (fscanf(In, "%d%lf%lf", &num, &x, &y) == 3) {
        p[i].num = num;
        p[i].x = x;
        p[i].y = y;
        i++;
    }
    fclose(In);
    
    quickHull(i);
    
    for (int j = 0; j < hullSize; j++) {
        fprintf(Out, "%d\n", hull[j].num);
    }
    fclose(Out);
    return 0;
}