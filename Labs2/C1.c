#include <stdio.h>
int main() {
    double matrix3x3[3][3];
    int matrix2x2[2][2];
    double result[2][2] = {0};
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

    printf("\n Введите 4 числа для матрицы 2 на 2 \n");
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j<2; j++)
        {
            scanf("%i", &matrix2x2[i][j]);
        }
    }

    for (i = 0; i<2; i++){
        for (j=0;j<2;j++){
            for (k = 0;k<2;k++){
                result[i][j] = matrix2x2[i][k]*matrix2x2[k][i];
            }

        }
    }

    printf("\nКвадрат матрицы:\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%.2f ", result[i][j]);
        }
        printf("\n");
    }



    return 0;
}   
