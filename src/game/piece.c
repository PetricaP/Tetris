#include "piece.h"
#include "game.h"
#include "piece_factory.h"
#include <framework/input_manager.h>
#include <framework/window.h>
#include <stdbool.h>
#include <stdio.h>

#define SMALL_Y_TIME_STEP 25
#define BIG_Y_TIME_STEP 200
#define X_TIME_STEP 100


static void player_change(Piece *piece, int *xSpeedP,
        unsigned int *yTimeStepP);
static bool is_colliding_piece_right(const Piece *piece, int border);
static bool is_colliding_piece_bottom(const Piece *piece, int border);
static bool is_colliding_piece_left(const Piece *piece, int border);
static void rotate_piece(Piece *piece);
static void update_piece_y(Piece *piece, Piece *nextPiece);

Piece create_piece(PieceType type, int x, int y, unsigned int blockWidth,
                   BlockColor color) {
    Piece piece;
    piece.x = x;
    piece.y = y;
    piece.blockWidth = blockWidth;
    piece.color = color;
    piece.type = type;
    piece.m_BlockTexture = get_texture(color);
    initialize_piece(&piece);
    return piece;
}

void update_piece(Piece *piece, Piece *nextPiece) {
    static unsigned int xPrevTicks = 0;
    static unsigned int yPrevTicks = 0;
    unsigned int yTimeStep = BIG_Y_TIME_STEP;
    const unsigned int xTimeStep = X_TIME_STEP;
    int xSpeed = 0;
    player_change(piece, &xSpeed, &yTimeStep);
    unsigned int ticks = SDL_GetTicks();
    if ((int)ticks - xPrevTicks > xTimeStep) {
        piece->x += xSpeed * piece->blockWidth;
        int i;
        for (i = 0; i < 4; ++i) {
            piece->blocks[i].rect.x += xSpeed * piece->blockWidth;
        }
        xPrevTicks = ticks;
        if ((int)ticks - yPrevTicks > yTimeStep) {
            update_piece_y(piece, nextPiece);
            yPrevTicks = ticks;
        }
    }
}

static void update_piece_y(Piece *piece, Piece *nextPiece) {
    int i = 0;
    bool hit_ground = false;
    piece->y += piece->blockWidth;
    for (i = 0; i < 4; ++i) {
        piece->blocks[i].rect.y += piece->blockWidth;
    }
    for (i = 0; i < 4; ++i) {
        if (is_colliding_piece_bottom(piece, get_screen_height())) {
            hit_ground = true;
            break;
        }
    }
    if (hit_ground) {
        piece->y -= piece->blockWidth;
        for (i = 0; i < 4; ++i) {
            piece->blocks[i].rect.y -= piece->blockWidth;
        }
        add_piece(piece);
        *piece = create_piece(nextPiece->type, get_grid_width() / 2,
                              -2 * piece->blockWidth + piece->blockWidth / 2,
                              piece->blockWidth, nextPiece->color);
        *nextPiece = create_piece(random() % PIECE_TYPES, get_grid_width() + 90,
                        piece->blockWidth * 10, piece->blockWidth, random() % 4);
    }
}

static void player_change(Piece *piece, int *xSpeedP, 
        unsigned int *yTimeStepP) {
    Piece copy = *piece;
    if (is_key_pressed(SDLK_LEFT)) {
        int i;
        copy.x -= copy.blockWidth;
        for (i = 0; i < 4; ++i) {
            copy.blocks[i].rect.x -= copy.blockWidth;
        }
        if (!is_colliding_piece_left(&copy, 0)) {
            *xSpeedP = -1;
        }
    }
    if(is_key_pressed(SDLK_RIGHT)) {
        int i;
        copy.x += copy.blockWidth;
        for (i = 0; i < 4; ++i) {
            copy.blocks[i].rect.x += copy.blockWidth;
        }
        if(!is_colliding_piece_right(&copy, get_grid_width())) {
            *xSpeedP = 1;
        }
    }
    if(is_key_pressed(SDLK_DOWN)) {
        *yTimeStepP = SMALL_Y_TIME_STEP;
    }
    static bool shouldRotate = false;
    if(shouldRotate && !is_key_pressed(SDLK_UP)) {
        shouldRotate = false;
        rotate_piece(piece);
    }
    if(is_key_pressed(SDLK_UP)) {
        shouldRotate = true;
    }
}

static bool is_colliding_piece(Piece *piece) {
    unsigned int i, j, k;
    unsigned int cols = get_cols();
    unsigned int rows = get_rows();
    for (k = 0; k < 4; ++k) {
        Block block = piece->blocks[k];
        if (block.rect.x + (int)block.rect.w > (int)get_grid_width() ||
            block.rect.x < 0 || block.rect.y > (int)get_screen_height()) {
            return true;
        }
        for (i = 0; i < rows; ++i) {
            for (j = 0; j < cols; ++j) {
                const Block *block2 = get_blocks(i, j);
                if (check_box_collision(&block.rect, &block2->rect)) {
                    return true;
                }
            }
        }
    }
    return false;
}

static void rotate_piece(Piece *piece) {
    if (piece->type != BLOCK) {
        Piece copy = *piece;
        for (int i = 0; i < 4; ++i) {
            int dx = copy.blocks[i].rect.x + copy.blockWidth - copy.x;
            int dy = copy.blocks[i].rect.y + copy.blockWidth - copy.y;
            copy.blocks[i].rect.x = copy.x - dy - copy.blockWidth;
            copy.blocks[i].rect.y = copy.y + dx - copy.blockWidth;
        }
        if (!is_colliding_piece(&copy)) {
            *piece = copy;
        }
    }
}

void draw_piece(const Piece *piece) {
    int i = 0;
    for (; i < 4; ++i) {
        draw_block(&piece->blocks[i]);
    }
}

static bool is_colliding_piece_right(const Piece *piece, int border) {
    int i = 0;
    for (; i < 4; ++i) {
        if(piece->blocks[i].rect.x + (int)piece->blocks[i].rect.w > border ||
            is_colliding_piece_blocks_right(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
}

static bool is_colliding_piece_left(const Piece *piece, int border) {
    int i = 0;
    for (; i < 4; ++i) {
        if (piece->blocks[i].rect.x < border ||
            is_colliding_piece_blocks_left(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
}

static bool is_colliding_piece_bottom(const Piece *piece, int border) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (piece->blocks[i].rect.y + (int)piece->blockWidth > border ||
            is_colliding_piece_blocks_bottom(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
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

