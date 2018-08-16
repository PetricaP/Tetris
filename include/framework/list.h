#ifndef FRAMEWORK_LIST_H
#define FRAMEWORK_LIST_H

#include "particle.h"
#include <SDL2/SDL_ttf.h>

typedef struct _Node Node;
typedef struct _Node_T Node_T;

struct _Node {
    Particle data;
    Node *next;
};

struct _Node_T {
    TTF_Font *data;
    char *key;
    Node_T *next;
};

void insert_node(Node **head, Particle data);
void remove_next_node(Node *nodePtr);
void remove_head(Node **head);

void insert_node_t(Node_T **head, const char *key, TTF_Font *data);

#endif // FRAMEWORK_LIST_H
