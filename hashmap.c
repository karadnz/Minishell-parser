#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct HashMap {
    int size;
    Node **buckets;
} HashMap;

unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

HashMap *hash_map_new(int size) {
    HashMap *hash_map = (HashMap *)malloc(sizeof(HashMap));
    hash_map->size = size;
    hash_map->buckets = (Node **)calloc(size, sizeof(Node *));
    return hash_map;
}

Node *node_new(const char *key, const char *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = strdup(key);
    node->value = strdup(value);
    node->next = NULL;
    return node;
}

void hash_map_insert(HashMap *hash_map, const char *key, const char *value) {
    unsigned long index = djb2_hash(key) % hash_map->size;
    Node *bucket = hash_map->buckets[index];
    Node *current = bucket;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }
    Node *new_node = node_new(key, value);
    new_node->next = bucket;
    hash_map->buckets[index] = new_node;
}

const char *hash_map_get(HashMap *hash_map, const char *key) {
    unsigned long index = djb2_hash(key) % hash_map->size;
    Node *current = hash_map->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void hash_map_free(HashMap *hash_map) {
    for (int i = 0; i < hash_map->size; i++) {
        Node *current = hash_map->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(hash_map->buckets);
    free(hash_map);
}

int main(int argc, char *argv[], char *envp[]) {
    HashMap *env_vars = hash_map_new(256);

    for (char **env = envp; *env != NULL; env++) {
        char *key = strtok(*env, "=");
        char *value = strtok(NULL, "=");
        hash_map_insert(env_vars, key, value);
    }

    // You can now access the environment variables using their hash values.
    printf("PATH: %s\n", hash_map_get(env_vars, "PATH"));

    hash_map_free(env_vars);
    return 0;
}
