#include "list.h"
#include <stdlib.h>
#include <string.h>

static Node *create_node(Particle data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->data = data;
    return node;
}

void insert_node(Node **head, Particle data) {
    if (*head == NULL) {
        *head = create_node(data);
    } else {
        Node *node = create_node(data);
        node->next = *head;
        *head = node;
    }
}

void remove_head(Node **head) {
    Node *next = (*head)->next;
    free(*head);
    *head = next;
}

static Node_T *create_node_t(const char *key, TTF_Font *data) {
    Node_T *node = (Node_T *)malloc(sizeof(Node_T));
    node->next = NULL;
    node->data = data;
    int l = strlen(key);
    node->key = (char *)malloc((l + 1) * sizeof(char));
    strncpy(node->key, key, l);
    node->key[l] = 0;
    return node;
}

void insert_node_t(Node_T **head, const char *key, TTF_Font *data) {
    if (*head == NULL) {
        *head = create_node_t(key, data);
    } else {
        Node_T *node = create_node_t(key, data);
        node->next = *head;
        *head = node;
    }
}

void remove_next_node(Node *nodePtr) {
    Node *temp = nodePtr->next;
    nodePtr->next = temp->next;
    free(temp);
}
