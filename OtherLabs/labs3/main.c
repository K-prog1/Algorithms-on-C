#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

typedef struct BookNode {
    char *author;
    char *title;
    char *publisher;
    int year;
    struct BookNode *left;
    struct BookNode *right;
} BOOK;

// Функция для обрезки пробелов
char* trim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

// Парсинг поля из BibTeX блока
char* get_field(char *block, const char *key) {
    char *p = strstr(block, key);
    if (!p) return NULL;
    
    // Ищем '=' после ключа
    p = strchr(p, '=');
    if (!p) return NULL;
    
    // Пропускаем '=' и пробелы до '{'
    p++;
    while (isspace((unsigned char)*p)) p++;
    if (*p != '{') return NULL;
    
    p++; // За '{'
    char *start = p;
    int depth = 1;
    
    // Ищем закрывающую '}' с учетом вложенности
    while (*p && depth > 0) {
        if (*p == '{') depth++;
        else if (*p == '}') depth--;
        p++;
    }
    
    int len = (p - start) - 1; // -1 чтобы не захватить саму '}'
    char *res = malloc(len + 1);
    if (!res) return NULL;
    
    memcpy(res, start, len);
    res[len] = '\0';
    
    return trim(res);
}

// Создание нового узла
BOOK* create_node(char *author, char *title, char *pub, int year) {
    BOOK *node = malloc(sizeof(BOOK));
    if (!node) return NULL;
    node->author = strdup(author ? author : "Unknown");
    node->title = strdup(title ? title : "Unknown");
    node->publisher = strdup(pub ? pub : "N/A");
    node->year = year;
    node->left = node->right = NULL;
    return node;
}

// Вставка в бинарное дерево
BOOK* insert(BOOK *root, char *author, char *title, char *pub, int year) {
    if (!root) {
        return create_node(author, title, pub, year);
    }

    // Сравнение: сначала автор, потом название
    int c = strcoll(author, root->author);
    if (c == 0) c = strcoll(title, root->title);

    if (c < 0) {
        root->left = insert(root->left, author, title, pub, year);
    } else if (c > 0) {
        root->right = insert(root->right, author, title, pub, year);
    } else {
        // Дубликат, обновляем год (или игнорируем)
        root->year = year;
    }
    return root;
}

// Рекурсивный вывод (обход слева направо)
void print_sorted(BOOK *root, FILE *out) {
    if (!root) return;
    print_sorted(root->left, out);
    fprintf(out, "Author: %s | Title: %s | Publisher: %s | Year: %d\n",
            root->author, root->title, root->publisher, root->year);
    print_sorted(root->right, out);
}

// Освобождение памяти
void free_tree(BOOK *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->author);
    free(root->title);
    free(root->publisher);
    free(root);
}

// Парсинг файла
void parse_bib(const char *filename, BOOK **root) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Сучка, файл '%s' не найден!\n", filename);
        return;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    if (len == 0) { fclose(f); return; }

    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0'; // Важно: добавляем нуль-терминатор
    fclose(f);

    char *p = buf;
    while (1) {
        // Ищем начало записи
        char *entry = strstr(p, "@");
        if (!entry) break;

        // Ищем открывающую скобку
        char *brace = strchr(entry, '{');
        if (!brace) break;

        // Ищем закрывающую скобку
        char *end = brace + 1;
        int depth = 1;
        while (*end && depth > 0) {
            if (*end == '{') depth++;
            else if (*end == '}') depth--;
            if (depth > 0) end++;
        }
        
        if (*end == '\0') break; // Неверный формат

        // Временно обрезаем строку, чтобы парсить только одну запись
        char temp = *end;
        *end = '\0';

        char *auth = get_field(brace, "author");
        char *title = get_field(brace, "title");
        char *pub = get_field(brace, "publisher");
        char *ystr = get_field(brace, "year");
        int year = ystr ? atoi(ystr) : 0;

        if (auth && title) {
            *root = insert(*root, auth, title, pub, year);
        }

        free(auth); free(title); free(pub); free(ystr);

        // Восстанавливаем символ и двигаемся дальше
        *end = temp;
        p = end + 1;
    }

    free(buf);
}

int main(int argc, char *argv[]) {
    // Для правильной сортировки кириллицы
    setlocale(LC_ALL, "");

    if (argc < 2) {
        printf("Usage: %s <file1.bib> [file2.bib ...]\n", argv[0]);
        return 1;
    }

    BOOK *root = NULL;
    for (int i = 1; i < argc; i++) {
        parse_bib(argv[i], &root);
    }

    if (!root) {
        fprintf(stderr, "Сучка, ничего не нашлось. Проверь файлы.\n");
        return 1;
    }

    FILE *out = fopen("sorted_bib.txt", "w");
    if (!out) {
        perror("Ошибка создания файла");
        return 1;
    }

    print_sorted(root, out);
    fclose(out);

    free_tree(root);
    printf("Готово. Проверь sorted_bib.txt.\n");
    return 0;
}