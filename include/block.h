#ifndef BLOCK_H_
#define BLOCK_H_

#include "collision.h"

#include <SDL2/SDL.h>

typedef enum {
    RED = 0, BLUE = 1, GREEN = 2, YELLOW = 3
} BlockColor;

typedef struct {
    Box2D rect;
    BlockColor color;
    SDL_Texture *m_Texture;
} Block;

Block create_block(SDL_Texture *texture, BlockColor color, int x, int y,
                   unsigned int width);
void draw_block(const Block *block);

#endif
