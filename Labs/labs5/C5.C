#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

int main(){
    double* A, *B;
    int i,j,n;
    char* sgn;

    system("chcp 1251");
    system("cls");

    printf("Введите размер матрицы:\n");
    scanf("%lf", n);


    A = (double*)malloc(n*n*sizeof(double));
    B = (double*)malloc(n*n*sizeof(double));

    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf("A[%lf][%lf] = ", i,j);
            scanf("%lf", (A + i*n + j));
        }
    }

    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf("B[%lf][%lf] = ", i,j);
            scanf("%lf", (B + i*n + j));
        }
    }
    printf("Введите + - * (сумма вычитание или умножение матриц %c \n)", sgn);
    scanf("%c", &sgn);
    double* C = calculate(A, B, sgn, n);
    
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            printf("%5lf", *(A + i*n + j));
        }
        printf("\n");
    }
    free(A); free(B); free(C)
}