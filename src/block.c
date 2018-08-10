#include "block.h"
#include "game.h"
#include "window.h"

#define MAX_BLOCKS 40

static Block m_Blocks[MAX_BLOCKS][MAX_BLOCKS / 2] = {{{{0, 0, 0, 0}, NULL}}};

void draw_blocks(void) {
    unsigned int i, j;
    for (i = 0; i < get_rows(); ++i) {
        for (j = 0; j < get_cols(); ++j) {
            Block block = m_Blocks[i][j];
            if (block.m_Texture != NULL) {
                draw_block(&block);
            }
        }
    }
}

const Block *get_blocks(unsigned int i, unsigned int j) {
    return &m_Blocks[i][j];
}

void add_piece(Piece *piece) {
    int i = 0;
    for (; i < 4; ++i) {
        Block block = piece->blocks[i];
        int col = block.rect.x / block.rect.w;
        int row = block.rect.y / block.rect.h;
        m_Blocks[row][col] = block;
    }
}

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
    SDL_Rect rect;
    rect.x = block->rect.x;
    rect.y = block->rect.y;
    rect.w = block->rect.w;
    rect.h = block->rect.h;
    SDL_RenderCopy(get_renderer(), block->m_Texture, NULL, &rect);
}

bool is_colliding_piece_blocks_left(const Block *block) {
    unsigned int i, j;
    unsigned int cols = get_cols();
    unsigned int rows = get_rows();
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            const Block *block2 = get_blocks(i, j);
            if (check_box_collision(&block->rect, &block2->rect)
                && block2->rect.x + (int)block2->rect.w >= block->rect.x) {
                return true;
            }
        }
    }
    return false;
}

bool is_colliding_piece_blocks_right(const Block *block) {
    unsigned int i, j;
    unsigned int cols = get_cols();
    unsigned int rows = get_rows();
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            const Block *block2 = get_blocks(i, j);
            if (check_box_collision(&block->rect, &block2->rect)
                && block->rect.x + (int)block->rect.w >= block2->rect.x) {
                return true;
            }
        }
    }
    return false;
}

bool is_colliding_piece_blocks_bottom(const Block *block) {
    unsigned int i, j;
    unsigned int cols = get_cols();
    unsigned int rows = get_rows();
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            const Block *block2 = get_blocks(i, j);
            if (check_box_collision(&block->rect, &block2->rect)
                && block->rect.y + (int)block->rect.h >= block2->rect.y
                && block->rect.x == block2->rect.x
                && block->rect.y < block2->rect.y + (int)block2->rect.h) {
                return true;
            }
        }
    }
    return false;
}

