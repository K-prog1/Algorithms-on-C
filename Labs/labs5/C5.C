#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

int main(){
    double* A, *B;
    int i,j,m,n,m2,n2;

    system("chcp 1251");
    system("cls");

    printf("Введите кол-во строк для первой матрицы:");
    scanf("%lf", n);
    printf("Введите кол-во столбцов для первой матрицы");
    scanf("%lf", m);

    printf("Введите кол-во строк для второй матрицы:");
    scanf("%lf", n2);
    printf("Введите кол-во столбцов для второй матрицы");
    scanf("%lf", m2);

    A = (double*)malloc(n*m*sizeof(double));
    B = (double*)malloc(n2*m2*sizeof(double));

    for (i = 0; i < n; i++ ){
        for (j = 0; j < m; j++){
            printf("A[%lf][%lf] = ", i,j);
            scanf("%lf", (A + i*m + j));
        }
    }

    for (i = 0; i < n2; i++ ){
        for (j = 0; j < m2; j++){
            printf("B[%lf][%lf] = ", i,j);
            scanf("%lf", (B + i*m2 + j));
        }
    }
    
}