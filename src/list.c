#include "list.h"
#include <stdlib.h>

static Node *create_node(Particle particle) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->data = particle;
    return node;
}

void insert_node(Node **head, Particle particle) {
    if(*head == NULL) {
        *head = create_node(particle);
    } else {
        Node *node = create_node(particle);
        node->next = *head;
        *head = node;
    }
}

void remove_head(Node **head) {
    Node *next = (*head)->next;
    if (next != NULL) {
        (*head)->data = next->data;
        (*head)->next = next->next;
        free(next);
    } else {
        free(*head);
        *head = NULL;
    }
}

void remove_next_node(Node *nodePtr) {
    Node *temp = nodePtr->next;
    nodePtr->next = temp->next;
    free(temp);
}
