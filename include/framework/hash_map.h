#ifndef FRAMEWORK_HASH_MAP_H
#define FRAMEWORK_HASH_MAP_H

#include "list.h"
#include <SDL2/SDL_ttf.h>

#define HASH_MAP_MAX_SIZE 13

int hash(const char *key);
void insert(Node_T *map[], const char *key, TTF_Font *data);
TTF_Font *get(Node_T *map[], const char *key);

#endif // FRAMEWORK_HASH_MAP_H
