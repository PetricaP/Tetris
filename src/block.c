#include "block.h"
#include "game.h"
#include "window.h"

Block create_block(SDL_Texture *texture, int x, int y,
                   unsigned int width) {
    Block block;
    block.rect.x = x;
    block.rect.y = y;
    block.rect.w = width;
    block.rect.h = width;
    block.m_Texture = texture;
    return block;
}

void draw_block(const Block *block) {
    SDL_RenderCopy(get_renderer(), block->m_Texture, NULL,
                   (SDL_Rect*)&block->rect);
}

