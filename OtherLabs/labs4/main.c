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

    while (1) {

        char in[MAX_INPUT_SIZE];
        printf("Enter input: ");
        scanf("\n%[^\n]", in); 

        char* tokens = tokenize(in);

        exprtree* expression = parse(tokens);

        int value = calculate(expression);

        printf("The result is: %d\n", value);

        free_exprtree(expression);

    }
    
    return 0;
}

char* tokenize(char* in) {

    char* tokens = malloc(sizeof(char) * MAX_INPUT_SIZE);

    int token_pos = 0; 


    int in_len = strlen(in);
    for (int i = 0; i < in_len; i++)

        if (strchr(VALID_TOKENS, in[i]))
            tokens[token_pos++] = in[i];

    tokens[token_pos] = '\0';

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

    if (expr->type == 'n')
        return expr->value;

    int left = calculate(expr->left);
    int right = calculate(expr->right);
    
    if (expr->type == '+')
        return left + right;
    else if (expr->type == '-')
        return left - right;
    else if (expr->type == '*')
        return left * right;
    else if (expr->type == '/')
        return left / right ? right : 0;

    return 0;
}

exprtree* parse_add_expression(parser_t* parser) {

    exprtree* expr = parse_mult_expression(parser);
    
    while (parser->pos < parser->ntokens &&
            (parser->tokens[parser->pos] == '+' || parser->tokens[parser->pos] == '-')) {

        char type = parser->tokens[parser->pos];

        parser->pos++;

        exprtree* right_expr = parse_mult_expression(parser);


        expr = create_exprtree(type, 0, expr, right_expr); 
    }
    
    return expr;

} 

exprtree* parse_mult_expression(parser_t* parser) {

    exprtree* expr = parse_atomic_expression(parser);
    
    while (parser->pos < parser->ntokens &&
            (parser->tokens[parser->pos] == '*' || parser->tokens[parser->pos] == '/')) {

        char type = parser->tokens[parser->pos];

        parser->pos++;

        exprtree* right_expr = parse_atomic_expression(parser);

        expr = create_exprtree(type, 0, expr, right_expr); 
    }
    
    return expr;

}

exprtree* parse_atomic_expression(parser_t* parser) {



    exprtree* expr;

    if (parser->tokens[parser->pos] == '(') {

        parser->pos++;


        expr = parse_add_expression(parser);

        if (parser->tokens[parser->pos] == ')')
            parser->pos++;
        else {
            
            fprintf(stderr, "Invalid input\n");
            exit(1);
        }
        
    } else {
 
        expr = parse_number(parser);
    }
    
    return expr;

}

exprtree* parse_number(parser_t* parser) {


    char number[MAX_INPUT_SIZE];
    int numberlen = 0;

    while (strchr("0123456789", parser->tokens[parser->pos]) &&
            numberlen < MAX_INPUT_SIZE && parser->pos < parser->ntokens) {

        number[numberlen++] = parser->tokens[parser->pos];
        parser->pos++;
    }
    number[numberlen] = '\0';

    if (numberlen == 0) {
        fprintf(stderr, "Invalid input, couldn't parse number\n");
        exit(1);
    }

    int value = atoi(number);
    
    exprtree* number_expr = create_exprtree('n', value, NULL, NULL);

    return number_expr;

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

    if (expr) {

        if (expr->left)
            free_exprtree(expr->left);
        if (expr->right)
            free_exprtree(expr->right);

        free(expr);

    }

}