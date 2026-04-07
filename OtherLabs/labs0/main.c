#include <stdio.h>
#include <stdlib.h>


int main() {
    int n;
    printf("\n Введите число, до которого вы собираетесь найти простые:  \n ");

    if (scanf("%i", &n) != 1 || n < 2 ) {
        printf("Натуральные значения не могут быть равны единицы или меньше 0");
        return 1;
    };

    int *P = malloc((n+1)*sizeof(int));

    if (!P) return 1;

    for (int i; i <= n; i++) 
    P[i] = i;

    int p = 2;
    
    while (p*p <= n){
        for (int j = p*p; j<=n; j += p) {
            P[j] = 0;
        }

        int first_alive_p = 0;
        for (int i = p + 1; i <= n; i ++){
            if (P[i] != 0){
                first_alive_p = P[i];
                break;
            }
        }
        
        if (first_alive_p == 0) break;
        p = first_alive_p;
    }

    int count = 0;
    for (int i = 2; i<=n; i++){
        if (P[i] != 0){
            P[count++] = P[i];
        }
    }
    printf("Простые числа до %d: ", n);
    for (int i = 0; i < count; i++) {
        printf("%d ", P[i]);
    }
    printf("\n");

    free(P);
    return 0;
}

