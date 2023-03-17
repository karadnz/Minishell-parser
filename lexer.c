#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TOKEN_PIPE,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_OUTPUT_APPEND,
    TOKEN_INPUT_HERE_DOC,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_WORD,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token get_next_token(const char **input) {
    Token token = {TOKEN_ERROR, NULL};

    while (isspace(**input)) {
        (*input)++;
    }

    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    const char *start = *input;
    char lookahead = *(*input + 1);

    switch (**input) {
        case '|':
            token.type = lookahead == '|' ? TOKEN_OR : TOKEN_PIPE;
            (*input) += lookahead == '|' ? 2 : 1;
            break;
        case '<':
            token.type = lookahead == '<' ? TOKEN_INPUT_HERE_DOC : TOKEN_INPUT;
            (*input) += lookahead == '<' ? 2 : 1;
            break;
        case '>':
            token.type = lookahead == '>' ? TOKEN_OUTPUT_APPEND : TOKEN_OUTPUT;
            (*input) += lookahead == '>' ? 2 : 1;
            break;
        case '&':
            token.type = lookahead == '&' ? TOKEN_AND : TOKEN_ERROR;
            (*input) += lookahead == '&' ? 2 : 1;
            break;
        default:
            token.type = TOKEN_WORD;
            while (!isspace(**input) && **input != '\0' && strchr("<>|&", **input) == NULL) {
                (*input)++;
            }
    }

    if (token.type == TOKEN_WORD) {
        size_t length = *input - start;
        token.value = malloc(length + 1);
        strncpy(token.value, start, length);
        token.value[length] = '\0';
    }

    return token;
}

int main(int argc, char **argv) {
    //const char *input = "cat file1 && sort < file2 | uniq > file3 || echo 'error'";
    Token token;

    do {
        token = get_next_token(&argv[1]);
        printf("Token type: %d, value: %s\n", token.type, token.value);
        free(token.value);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);

    return 0;
}