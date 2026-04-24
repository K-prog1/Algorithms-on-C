#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct parser_t {
    char* tokens;
    int ntokens;
    int pos;
} parser_t;

typedef struct exprtree {
    char type;
    int value;
    struct exprtree* left;
    struct exprtree* right;
} exprtree;

#define VALID_TOKENS "+-*/0123456789()"
#define MAX_INPUT_SIZE 100

char* tokenize(char*);
exprtree* parse(char*);
int calculate(exprtree*);

static void free_exprtree(exprtree*);
static exprtree* create_exprtree(char, int, exprtree*, exprtree*);
exprtree* parse_add_expression(parser_t* parser);
exprtree* parse_mult_expression(parser_t* parser);
exprtree* parse_atomic_expression(parser_t* parser);
exprtree* parse_number(parser_t* parser);

int main(int argc, char* argv[]) {
    char in[MAX_INPUT_SIZE];
    while (1) {
        printf("Enter input (or 'q' to quit): ");
        if (!fgets(in, sizeof(in), stdin)) break;
        if (in[0] == 'q' || in[0] == '\n') break;

        char* tokens = tokenize(in);
        if (!tokens || strlen(tokens) == 0) { free(tokens); continue; }

        exprtree* expression = parse(tokens);
        int value = calculate(expression);
        printf("The result is: %d\n", value);

        free_exprtree(expression);
        free(tokens);
    }
    return 0;
}

// strtok разбивает строку по пробелам/табам/переносам.
// Остальные символы собираем в плоский массив, как требует твой парсер.
char* tokenize(char* in) {
    char* tokens = malloc(MAX_INPUT_SIZE);
    int pos = 0;
    char* copy = strdup(in); // strtok меняет строку, копируем чтобы не сломать in
    if (!copy) return NULL;

    char* tok = strtok(copy, " \t\n\r");
    while (tok != NULL) {
        for (int i = 0; tok[i]; i++) {
            if (strchr(VALID_TOKENS, tok[i])) {
                tokens[pos++] = tok[i];
            }
        }
        tok = strtok(NULL, " \t\n\r");
    }
    tokens[pos] = '\0';
    free(copy);
    return tokens;
}

exprtree* parse(char* tokens) {
    parser_t* parser = malloc(sizeof(parser_t));
    parser->tokens = tokens;
    parser->ntokens = strlen(tokens);
    parser->pos = 0;

    exprtree* expression = parse_add_expression(parser);

    free(parser->tokens);
    free(parser);
    return expression;
}

int calculate(exprtree* expr) {
    if (expr->type == 'n') return expr->value;

    int left = calculate(expr->left);
    int right = calculate(expr->right);
    
    if (expr->type == '+') return left + right;
    if (expr->type == '-') return left - right;
    if (expr->type == '*') return left * right;
    if (expr->type == '/') return right != 0 ? left / right : 0; // починила деление

    return 0;
}

exprtree* parse_add_expression(parser_t* p) {
    exprtree* expr = parse_mult_expression(p);
    while (p->pos < p->ntokens && (p->tokens[p->pos] == '+' || p->tokens[p->pos] == '-')) {
        char type = p->tokens[p->pos++];
        exprtree* right = parse_mult_expression(p);
        expr = create_exprtree(type, 0, expr, right);
    }
    return expr;
}

exprtree* parse_mult_expression(parser_t* p) {
    exprtree* expr = parse_atomic_expression(p);
    while (p->pos < p->ntokens && (p->tokens[p->pos] == '*' || p->tokens[p->pos] == '/')) {
        char type = p->tokens[p->pos++];
        exprtree* right = parse_atomic_expression(p);
        expr = create_exprtree(type, 0, expr, right);
    }
    return expr;
}

exprtree* parse_atomic_expression(parser_t* p) {
    if (p->pos < p->ntokens && p->tokens[p->pos] == '(') {
        p->pos++;
        exprtree* expr = parse_add_expression(p);
        if (p->pos < p->ntokens && p->tokens[p->pos] == ')') p->pos++;
        else { fprintf(stderr, "Invalid input: missing ')'\n"); exit(1); }
        return expr;
    }
    return parse_number(p);
}

exprtree* parse_number(parser_t* p) {
    char num[MAX_INPUT_SIZE];
    int len = 0;
    while (p->pos < p->ntokens && strchr("0123456789", p->tokens[p->pos]) && len < MAX_INPUT_SIZE - 1) {
        num[len++] = p->tokens[p->pos++];
    }
    num[len] = '\0';
    if (len == 0) { fprintf(stderr, "Invalid input: expected number\n"); exit(1); }
    return create_exprtree('n', atoi(num), NULL, NULL);
}

static exprtree* create_exprtree(char type, int value, exprtree* left, exprtree* right) {
    exprtree* expr = malloc(sizeof(exprtree));
    expr->type = type;
    expr->value = value;
    expr->left = left;
    expr->right = right;
    return expr;
}

static void free_exprtree(exprtree* expr) {
    if (!expr) return;
    free_exprtree(expr->left);
    free_exprtree(expr->right);
    free(expr);
}