#include <stdio.h>

int main(void)
{
    int age = 25;
    float height = 1.75;
    char grade = 'A';
    int numbers[3] = {10, 20, 30};
    
    // Вывод значений
    printf("Values:\n");
    printf("age = %d\n", age);
    printf("height = %.2f\n", height);
    printf("grade = %c\n", grade);
    
    // Вывод АДРЕСОВ этих переменных в памяти
    printf("\nAddresses in memory:\n");
    printf("&age = %p\n", &age);
    printf("&height = %p\n", &height);
    printf("&grade = %p\n", &grade);
    printf("&numbers = %p\n", &numbers);
    printf("&numbers[0] = %p\n", &numbers[0]);
    printf("&numbers[1] = %p\n", &numbers[1]);
    
    // Пример со scanf - вводим данные по адресу
    printf("\nEnter new age: ");
    scanf("%d", &age);  // передаем АДРЕС, чтобы scanf мог ИЗМЕНИТЬ переменную
    
    printf("New age = %d\n", age);
    
    return 0;
}