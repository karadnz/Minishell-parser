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


typedef enum {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_INPUT,
    NODE_OUTPUT,
    NODE_OUTPUT_APPEND,
    NODE_INPUT_HERE_DOC
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *left;
    struct Node *right;
    Token token;
} Node;




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

    if (1) {
        size_t length = *input - start;
        token.value = malloc(length + 1);
        strncpy(token.value, start, length);
        token.value[length] = '\0';
    }

    return token;
}

/*
if (token.type == TOKEN_WORD) {
        size_t length = *input - start;
        token.value = malloc(length + 1);
        strncpy(token.value, start, length);
        token.value[length] = '\0';
    }*/







Node *parse_command(const char **input);
Node *parse_pipeline(const char **input);
Node *parse_and(const char **input);
Node *parse_or(const char **input);
Node *parse_input(const char **input);
Node *parse_output(const char **input);
Node *parse_output_append(const char **input);

Node *create_node(NodeType type, Node *left, Node *right, Token token) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->right = right;
    node->token = token;
    return node;
}

/*Node *parse_command(const char **input) {
    Token token = get_next_token(input);
    if (token.type == TOKEN_WORD) {
        return create_node(NODE_COMMAND, NULL, NULL, token);
    } else {
        return NULL;
    }
}*/

/*Node *parse_command(const char **input) {
    Token token = get_next_token(input);
    if (token.type == TOKEN_WORD) {
        return create_node(NODE_COMMAND, NULL, NULL, token);
    } else {
        *input -= token.type == TOKEN_ERROR ? 1 : strlen(token.value) + 1;
        return NULL;
    }
}*/

Node *parse_command(const char **input) {
    Token token = get_next_token(input);
    if (token.type == TOKEN_WORD) {
        return create_node(NODE_COMMAND, NULL, NULL, token);
    } else {
        if (token.value != NULL) {
            *input -= strlen(token.value) + 1;
        } else {
            *input -= token.type == TOKEN_ERROR ? 1 : 0;
        }
        return NULL;
    }
}

Node *parse_pipeline(const char **input) {
    Node *left = parse_command(input);
    if (!left) return NULL;

    Token token = get_next_token(input);
    if (token.type == TOKEN_PIPE) {
        Node *right = parse_pipeline(input);
        if (!right) {
            // Error: Malformed pipeline
            return NULL;
        }
        return create_node(NODE_PIPE, left, right, token);
    } else {
        // Unprocessed token, put it back to input
        *input -= strlen(token.value) + 1;
        return left;
    }
}

Node *parse_and(const char **input) {
    Node *left = parse_pipeline(input);
    if (!left) return NULL;

    Token token = get_next_token(input);
    if (token.type == TOKEN_AND) {
        Node *right = parse_and(input);
        if (!right) {
            // Error: Malformed and
            return NULL;
        }
        return create_node(NODE_AND, left, right, token);
    } else {
        // Unprocessed token, put it back to input
        *input -= strlen(token.value) + 1;
        return left;
    }
}

Node *parse_or(const char **input) {
    Node *left = parse_and(input);
    if (!left) return NULL;

    Token token = get_next_token(input);
    if (token.type == TOKEN_OR) {
        Node *right = parse_or(input);
        if (!right) {
            // Error: Malformed or
            return NULL;
        }
        return create_node(NODE_OR, left, right, token);
    } else {
        // Unprocessed token, put it back to input
        *input -= strlen(token.value) + 1;
        return left;
    }
}

Node *parse_input(const char **input) {
    Node *left = parse_or(input);
    if (!left) return NULL;

    Token token = get_next_token(input);
    if (token.type == TOKEN_INPUT || token.type == TOKEN_INPUT_HERE_DOC) {
        Node *right = parse_command(input);
        if (!right) {
            // Error: Malformed
        // Error: Malformed input redirection
        return NULL;
    }
    return create_node(token.type == TOKEN_INPUT ? NODE_INPUT : NODE_INPUT_HERE_DOC, left, right, token);
} else {
    // Unprocessed token, put it back to input
    *input -= strlen(token.value) + 1;
    return left;
}
}

Node *parse_output(const char **input) {
    Node *left = parse_input(input);
    if (!left) return NULL;

    Token token = get_next_token(input);
    if (token.type == TOKEN_OUTPUT || token.type == TOKEN_OUTPUT_APPEND) {
        Node *right = parse_command(input);
        if (!right) {
            // Error: Malformed output redirection
            return NULL;
        }
        return create_node(token.type == TOKEN_OUTPUT ? NODE_OUTPUT : NODE_OUTPUT_APPEND, left, right, token);
    } else {
        // Unprocessed token, put it back to input
        *input -= strlen(token.value) + 1;
        return left;
    }
}

Node *parse(const char **input) {
    return parse_output(input);
}
void print_ast(Node *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }

    printf("%d", node->type);
    if (node->token.value) {
        printf(" (%s)", node->token.value);
    }
    printf("\n");

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

int main(int argc, char **argv) {
    const char *input = "cat <<eof >file1 && cat file1 && abc || wc <file1 | cat >file2'";
    Node *ast = parse(&input);
    print_ast(ast, 0);

    return 0;
}


/*int main(int argc, char **argv) {
    //const char *input = "cat file1 && sort < file2 | uniq > file3 || echo 'error'";
    Token token;

    do {
        token = get_next_token(&argv[1]);
        printf("Token type: %d, value: %s\n", token.type, token.value);
        free(token.value);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);

    return 0;
}*/