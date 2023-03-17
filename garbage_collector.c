#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    void *ptr;
    struct Block *next;
} Block;

static Block *head = NULL;

void *gc_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    Block *new_block = (Block *)malloc(sizeof(Block));
    if (new_block == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_block->ptr = ptr;
    new_block->next = head;
    head = new_block;

    return ptr;
}

void gc_free(void *ptr) {
    Block *current = head;
    Block *prev = NULL;

    while (current != NULL) {
        if (current->ptr == ptr) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                head = current->next;
            }

            free(current->ptr);
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }

    fprintf(stderr, "Pointer not found in garbage collector\n");
}

void gc_cleanup() {
    Block *current = head;
    Block *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->ptr);
        free(temp);
    }

    head = NULL;
}

int main() {
    // Example usage of the garbage collector
    char *str = (char *)gc_malloc(256 * sizeof(char));
    snprintf(str, 256, "This is an example string in minishell!");

    printf("%s\n", str);

    gc_cleanup(); // Free all allocated memory

    return 0;
}
