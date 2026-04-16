#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>

typedef struct word {
  int count;
  char *Word;
 } WORD;
 
int cmpword_alpha(WORD *, WORD *);
int cmpword_quant(WORD *, WORD *);

WORD words[100000];

int totalw=0;

int main(int ac, char *av[])
{
  FILE *OUT1, *OUT2, *IN;
  char *pword;
  // здесь открыть файлы для вывода,
  // проверка на успех,  инициализация переменных и т.д.
  if (ac<2)
  {
    printf ("Вы забыли указать список файлов!\n"); exit (0);
  }
  while (--ac) // идем по списку файлов
  {
   IN=fopen(av[ac],"r");
   printf("Обрабатывается файл %s\n",av[ac]);
   // открываем очередной файл
   while ((pword=getword(IN))!=NULL)
    { // .........................
      // работаем с прочитанным словом
      // добавляем в массив с увеличением totalw
      // или увеличиваем счетчик уже
      // занесенного в словарь слова
      //
      //     break;  // только для проверки
    }
   fclose(IN);
  }
 qsort(words,totalw,sizeof(WORD *),
        (int (*)(const void *, const void *))cmpword_alpha );
 // здесь вывод массива
 qsort(words,totalw,sizeof(WORD *),
        (int (*)(const void *, const void *))cmpword_quant );
 // здесь вывод массива
return 0; 
}
char *getword(FILE *F)
{ char *s; int c;
 // считываем одно слово из файла F
 //................. 
 // return NULL если файл кончился
 return s;
}
int cmpword_alpha(WORD *w1,  WORD *w2)
{
  ;
}
int cmpword_quant(WORD *w1,  WORD *w2)
{
  ;
}
