#include <stdio.h>
int main() {
    double matrix3x3[3][3];
    int i,j,k;
    double summMain = 0, summDiagonal = 0;
    printf("\n Введите 9 чисел для матрицы 3 на 3 \n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j<3; j++)
        {
            scanf("%lf", &matrix3x3[i][j]);
        }
    }
    for (i = 0; i < 3; i++)
    {

        summMain += matrix3x3[i][i];
        summDiagonal += matrix3x3[i][2-i];
    }
    printf("сумма основной диагонали: %lf", summMain);
    printf("сумма вторичной диагонали: %lf", summDiagonal);

    return 0;
}   
