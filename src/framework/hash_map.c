#include "hash_map.h"
#include <stdio.h>
#include <string.h>

int hash(const char *key) {
    int count = 0;
    while (*key) {
        count += *key;
        ++key;
    }
    return count % HASH_MAP_MAX_SIZE;
}

void insert(Node_T *map[], const char *key, TTF_Font *data) {
    int index = hash(key);
    insert_node_t(&map[index], key, data);
}

TTF_Font *get(Node_T *map[], const char *key) {
    int index = hash(key);
    Node_T *node = map[index];
    if (node == NULL) {
        return NULL;
    }
    while (strcmp(node->key, key) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node->data;
}
