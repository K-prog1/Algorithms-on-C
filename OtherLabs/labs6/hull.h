#ifndef HULL_H
#define HULL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point {
    double x;
    double y;
    int num;
} POINT;

extern POINT p[100000];
extern POINT hull[100000];
extern int hullSize;

int cmpX(const void* a, const void* b);
double cross(POINT A, POINT B, POINT C);
int side(POINT A, POINT B, POINT C);
void findHull(int n, POINT A, POINT B, int sideVal, POINT* subset);
void quickHull(int n);

#endif