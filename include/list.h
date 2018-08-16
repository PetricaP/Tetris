#ifndef LIST_H
#define LIST_H

#include "particle.h"

typedef struct _Node Node;

struct _Node {
    Particle data;
    Node *next;
};

void insert_node(Node **head, Particle particle);
void remove_next_node(Node *nodePtr);
void remove_head(Node **head);

#endif // LIST_H
