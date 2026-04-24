#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BookNode {
    char *author;
    char *title;
    char *publisher;
    int year;
    struct BookNode *left;
    struct BookNode *right;
} BOOK;


char* get_field(char *block, const char *key) {
    char *p = strstr(block, key);
    if (!p) return NULL;
    p = strchr(p, '=');
    if (!p) return NULL;
    p++;
    while (*p && (*p == ' ' || *p == '\t')) p++;
    if (*p != '{') return NULL;
    p++;

    char *end = p;
    int depth = 1;
    
    while (*end && depth > 0) {
        if (*end == '{') depth++;
        else if (*end == '}') depth--;
        if (depth > 0) end++;
    }
    if (!end) return NULL;
    
    int len = end - p;
    char *res = malloc(len + 1);
    memcpy(res, p, len);
    res[len] = '\0';
    

    char *s = res, *e = res + len - 1;
    while (*s == ' ') s++;
    while (e > s && *e == ' ') e--;
    *(e + 1) = '\0';
    if (s != res) memmove(res, s, strlen(s) + 1);
    return res;
}


int cmp_books(const char *a1, const char *t1, const char *a2, const char *t2) {
    int res = strcmp(a1, a2);
    if (res != 0) return res;
    return strcmp(t1,t2);
   
}


BOOK* insert(BOOK *root, char *auth, char *title, char *pub, int year) {
    if (!root) {
        root = malloc(sizeof(BOOK));
        root->author   = malloc(strlen(auth) + 1);   strcpy(root->author, auth);
        root->title    = malloc(strlen(title) + 1);  strcpy(root->title, title);
        root->publisher= malloc(strlen(pub ? pub : "N/A") + 1); strcpy(root->publisher, pub ? pub : "N/A");
        root->year = year;
        root->left = root->right = NULL;
        return root;
    }
    
    int c = cmp_books(auth, title, root->author, root->title);
    if (c < 0) root->left  = insert(root->left, auth, title, pub, year);
    else if (c > 0) root->right = insert(root->right, auth, title, pub, year);
    else root->year = year; 
    return root;
}

BOOK* find_prefix(BOOK *root, const char *prefix) {
    if (!root) return NULL;
    int res = strncmp(root->author, prefix, 5);
    if (res == 0) return root;
    return (res > 0) ? find_prefix(root->left, prefix) : find_prefix(root->right, prefix);
}

void print_sorted(BOOK *root, FILE *out) {
    if (!root) return;
    print_sorted(root->left, out);
    fprintf(out, "Author: %s | Title: %s | Publisher: %s | Year: %d\n",
            root->author, root->title, root->publisher, root->year);
    print_sorted(root->right, out);
}

void free_tree(BOOK *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->author); free(root->title); free(root->publisher);
    free(root);
}

void parse_bib(const char *filename, BOOK **root) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror(filename); return; }
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);

    char *p = buf;
    while ((p = strstr(p, "@")) != NULL) {
        char *end = strchr(p, '}');
        if (!end) break;
        *end = '\0';
        
        char *auth = get_field(p, "author");
        char *title = get_field(p, "title");
        char *pub  = get_field(p, "publisher");
        char *ystr = get_field(p, "year");
        int year = ystr ? atoi(ystr) : 0;

        if (auth && title) *root = insert(*root, auth, title, pub, year);
        
        free(auth); free(title); free(pub); free(ystr);
        p = end + 1;
    }
    free(buf);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s file1.bib [file2.bib ...]\n", argv[0]);
        return 1;
    }
    
    BOOK *root = NULL;
    for (int i = 1; i < argc; i++) parse_bib(argv[i], &root);

    FILE *out = fopen("sorted_bib.txt", "w");
    if (!out) { perror("output"); return 1; }
    
    print_sorted(root, out);
    fclose(out);
    
    BOOK *found = find_prefix(root, "Pushk"); 
    if (found) printf("Found prefix match: %s\n", found->author);

    free_tree(root);
    return 0;
}