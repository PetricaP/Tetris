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
void draw_blocks(void);
bool is_colliding_piece_blocks_left(const Block *block);
bool is_colliding_piece_blocks_right(const Block *block);
bool is_colliding_piece_blocks_bottom(const Block *block);

#endif
