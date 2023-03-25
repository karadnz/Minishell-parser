#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*builtin_func)(int, char **);

typedef struct {
    const char *name;
    builtin_func func;
} Builtin;

void echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

void cd(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(argv[1]) != 0) {
        perror("cd");
    }
}

void pwd(int argc, char *argv[]) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

void shell_exit(int argc, char *argv[]) {
    exit(0);
}

void help(int argc, char *argv[]);

Builtin builtins[] = {
    {"echo", echo},
    {"cd", cd},
    {"pwd", pwd},
    {"exit", shell_exit},
    {"help", help},
};

int num_builtins() {
    return sizeof(builtins) / sizeof(Builtin);
}

void help(int argc, char *argv[]) {
    printf("Shell builtins:\n");
    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtins[i].name);
    }
}

unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

builtin_func find_builtin(const char *name) {
    unsigned long index = djb2_hash(name);
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(builtins[i].name, name) == 0 && djb2_hash(builtins[i].name) == index) {
            return builtins[i].func;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        builtin_func func = find_builtin(argv[1]);
        if (func) {
            func(argc - 1, &argv[1]);
        } else {
            printf("Builtin not found: %s\n", argv[1]);
        }
    }
    return 0;
}
