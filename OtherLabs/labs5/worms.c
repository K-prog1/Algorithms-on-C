#include <stdio.h>
#include <stdlib.h>

#define MAXN 100001

/* Список смежности дерева */
int to[MAXN], edge_len[MAXN], next_edge[MAXN], head[MAXN];
/* depth[v] = суммарная длина пути от корня до v */
long long depth[MAXN];
/* last_tail[v] = глубина хвоста последнего червяка на пути от корня до v */
long long last_tail[MAXN];
int ans = 0;
int n, L;

/* Итеративный DFS (без рекурсии — до 10^5 вершин, стек ОС мал) */
int stk[MAXN];    /* стек вершин   */
int par[MAXN];    /* родитель       */

void solve(void) {
    int top = 0;
    stk[top++] = 0;
    par[0] = -1;
    depth[0] = 0;
    last_tail[0] = -(long long)L; /* у корня (глубина 0) можно разместить червяка */

    while (top > 0) {
        int v = stk[--top];

        /* Пробуем разместить червяка: хвост = depth[v] - L */
        long long head_pos = depth[v];
        long long tail_pos = head_pos - L;
        /* last_tail[v] — где кончается предыдущий червяк на этом пути.
           Если tail_pos >= last_tail[v], червяки не перекрываются. */
        if (tail_pos >= last_tail[v]) {
            ans++;
            last_tail[v] = head_pos; /* хвост нового червяка = depth[v] */
        }

        /* Обходим детей */
        for (int e = head[v]; e != -1; e = next_edge[e]) {
            int u = to[e];
            if (u == par[v]) continue;
            par[u] = v;
            depth[u] = depth[v] + edge_len[e];
            last_tail[u] = last_tail[v]; /* наследуем состояние пути */
            stk[top++] = u;
        }
    }
}

int main(void) {
    scanf("%d %d", &n, &L);

    for (int i = 0; i < n; i++) head[i] = -1;

    /* Читаем A[0..N-2] */
    int A[MAXN];
    for (int i = 0; i < n - 1; i++) scanf("%d", &A[i]);

    /* Читаем B[0..N-2] */
    for (int i = 0; i < n - 1; i++) {
        int b;
        scanf("%d", &b);
        /* Ребро: A[i] — i+1, длина b */
        int u = A[i], v = i + 1;
        /* Добавляем оба направления */
        to[i]        = v; edge_len[i]        = b; next_edge[i]        = head[u]; head[u] = i;
        to[i+n]      = u; edge_len[i+n]      = b; next_edge[i+n]      = head[v]; head[v] = i+n;
    }

    solve();

    printf("%d\n", ans);
    return 0;
}