#include <stdio.h>
#include <string.h>

int main() {
    char text[] = "Пример";
    
    // strlen считает символы
    printf("Символов: %zu\n", strlen(text)); // Выведет 6 (для UTF-8 может быть иначе)
    
    // sizeof считает байты, включая '\0'
    printf("Размер массива: %zu\n", sizeof(text)); // Выведет 7 (6 + 1)
    
    return 0;
}
