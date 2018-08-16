#include "hash_map.h"
#include <stdio.h>
#include <string.h>

int hash(const char *key) {
    int count = 0;
    while (*key) {
        count += *key;
    }
    return count % HASH_MAP_MAX_SIZE;
}

void insert(Node_T *map[], const char *key, TTF_Font *data) {
    int index = hash(key);
    insert_node_t(&map[index], key, data);
}

TTF_Font *get(Node_T *map[], const char *key) {
    int index = hash(key);
    if (map[index] == NULL) {
        fprintf(stderr, "No such entry in map.");
        return NULL;
    } else {
        Node_T *node = map[index];
        while (!strcmp(node->key, key)) {
            node = node->next;
        }
        if (node == NULL) {
            fprintf(stderr, "No such entry in map.");
            return NULL;
        }
        return node->data;
    }
}

