#include "hull.h"

POINT p[100000];
POINT hull[100000];
int hullSize = 0;

int cmpX(const void* a, const void* b) {
    POINT* p1 = (POINT*)a;
    POINT* p2 = (POINT*)b;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

double cross(POINT A, POINT B, POINT C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

int side(POINT A, POINT B, POINT C) {
    double res = cross(A, B, C);
    if (res > 1e-9) return 1;
    if (res < -1e-9) return -1;
    return 0;
}

void findHull(int n, POINT A, POINT B, int sideVal, POINT* subset) {
    if (n == 0) return;
    int maxIdx = -1;
    double maxDist = 0;
    for (int i = 0; i < n; i++) {
        if (side(A, B, subset[i]) == sideVal) {
            double d = fabs(cross(A, B, subset[i]));
            if (d > maxDist) {
                maxDist = d;
                maxIdx = i;
            }
        }
    }
    if (maxIdx == -1) return;
    hull[hullSize++] = subset[maxIdx];
    
    POINT* leftAC = malloc(n * sizeof(POINT));
    POINT* leftCB = malloc(n * sizeof(POINT));
    int n1 = 0, n2 = 0;
    for (int i = 0; i < n; i++) {
        if (i == maxIdx) continue;
        if (side(A, subset[maxIdx], subset[i]) == sideVal) leftAC[n1++] = subset[i];
        else if (side(subset[maxIdx], B, subset[i]) == sideVal) leftCB[n2++] = subset[i];
    }
    findHull(n1, A, subset[maxIdx], sideVal, leftAC);
    findHull(n2, subset[maxIdx], B, sideVal, leftCB);
    free(leftAC);
    free(leftCB);
}

void quickHull(int n) {
    if (n < 3) {
        for(int i = 0; i < n; i++) hull[hullSize++] = p[i];
        return;
    }
    qsort(p, n, sizeof(POINT), cmpX);
    POINT minP = p[0];
    POINT maxP = p[n-1];
    hull[hullSize++] = minP;
    
    POINT* upper = malloc(n * sizeof(POINT));
    POINT* lower = malloc(n * sizeof(POINT));
    int nU = 0, nL = 0;
    for(int i = 1; i < n - 1; i++) {
        if (side(minP, maxP, p[i]) == 1) upper[nU++] = p[i];
        else if (side(minP, maxP, p[i]) == -1) lower[nL++] = p[i];
    }
    findHull(nU, minP, maxP, 1, upper);
    hull[hullSize++] = maxP;
    findHull(nL, minP, maxP, -1, lower);
    free(upper);
    free(lower);
}