#include <stdio.h>
#include <string.h>

int main(){
    char data[81];
    int i;
    
    printf("\nВведите строку:\n");
    fgets(data, sizeof(data), stdin);

    for (i = 0; data[i] != '\0'; i++) {
   
        if (data[i] == 'a'){
            data[i] = 'A';  
        }
        else if (data[i] == 'b')  
        {
            data[i] = 'B';  
        }
    }
    
    printf("Результат: %s\n", data);
    return 0;
}