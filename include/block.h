#ifndef BLOCK_H_
#define BLOCK_H_

#include "collision.h"

#include <SDL2/SDL.h>

typedef struct {
    Box2D rect;
    SDL_Texture *m_Texture;
} Block;

Block create_block(SDL_Texture *texture, int x, int y,
                   unsigned int width);
void draw_block(const Block *block);

#endif
