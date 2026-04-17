#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PEOPLE 100
#define MAX_NAME 64

typedef struct Person {
    char name[MAX_NAME];
    int year;
    char gender;
    double height;
} Person;

struct Person data[MAX_PEOPLE]; 
int count = 0;
int sort_keys[8];
int sorl_len = 0;

int cmp(const void *a, const void *b) {
    const Person *p1 = (const Person *)a;
    const Person *p2 = (const Person *)b;
};