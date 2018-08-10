#include "piece.h"
#include "game.h"
#include "input_manager.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#define SMALL_Y_TIME_STEP 50
#define BIG_Y_TIME_STEP 300
#define X_TIME_STEP 100

static SDL_Texture *m_Textures[4] = { NULL };

/*  000  */
/*  010  */
/*  000  */
static void initialize_blocks_center(Piece *piece) {
    piece->blocks[0] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y - piece->blockWidth / 2,
                                    piece->blockWidth);
}

/*  000  */
/*  111  */
/*  000  */
static void initialize_blocks_bed(Piece *piece) {
    initialize_blocks_center(piece);

    piece->blocks[1] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth * 3 / 2,
                                    piece->y - piece->blockWidth / 2,
                                    piece->blockWidth);

    piece->blocks[2] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y - piece->blockWidth / 2,
                                    piece->blockWidth);
}

/*  100  */
/*  111  */
/*  000  */
static void initialize_blocks_lbed(Piece *piece) {
    initialize_blocks_bed(piece);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth * 3 / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);
}

/*  001  */
/*  111  */
/*  000  */
static void initialize_blocks_rbed(Piece *piece) {
    initialize_blocks_bed(piece);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);
}

/*  010  */
/*  111  */
/*  000  */
static void initialize_blocks_table(Piece *piece) {
    initialize_blocks_bed(piece);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);
}

/*  001  */
/*  011  */
/*  010  */
static void initialize_blocks_rchair(Piece *piece) {
    initialize_blocks_center(piece);

    piece->blocks[1] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y - piece->blockWidth / 2,
                                    piece->blockWidth);

    piece->blocks[2] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y + piece->blockWidth / 2,
                                    piece->blockWidth);
}

/*  100  */
/*  110  */
/*  010  */
static void initialize_blocks_lchair(Piece *piece) {
    initialize_blocks_center(piece);

    piece->blocks[1] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y - piece->blockWidth / 2,
                                    piece->blockWidth);

    piece->blocks[2] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x + piece->blockWidth / 2,
                                    piece->y + piece->blockWidth / 2,
                                    piece->blockWidth);
}

/*  010  */
/*  010  */
/*  010  */
/*  010  */
static void initialize_blocks_pillar(Piece *piece) {
    initialize_blocks_center(piece);

    piece->blocks[1] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y + piece->blockWidth / 2,
                                    piece->blockWidth);

    piece->blocks[2] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 3 / 2,
                                    piece->blockWidth);

    piece->blocks[3] = create_block(piece->m_BlockTexture,
                                    piece->x - piece->blockWidth / 2,
                                    piece->y - piece->blockWidth * 5 / 2,
                                    piece->blockWidth);
}

static void initialize_blocks(Piece *piece) {
    switch (piece->type) {
        case LBED:
            initialize_blocks_lbed(piece);
            break;

        case RBED:
            initialize_blocks_rbed(piece);
            break;

        case TABLE:
            initialize_blocks_table(piece);
            break;

        case LCHAIR:
            initialize_blocks_lchair(piece);
            break;

        case RCHAIR:
            initialize_blocks_rchair(piece);
            break;

        case PILLAR:
            initialize_blocks_pillar(piece);
            break;
    }
}

Piece create_piece(PieceType type, int x, int y, unsigned int blockWidth,
        BlockColor color) {
    Piece piece;

    piece.x = x;
    piece.y = y;
    piece.blockWidth = blockWidth;
    piece.color = color;
    piece.type = type;
    piece.m_BlockTexture = m_Textures[color];
    initialize_blocks(&piece);

    return piece;
}

void set_texture_source(const char *redTexturePath, 
                        const char *blueTexturePath,
                        const char *greenTexturePath,
                        const char *yellowTexturePath) {

    m_Textures[RED] = IMG_LoadTexture(get_renderer(),
            redTexturePath);
    if (m_Textures[RED] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", redTexturePath);
    }

    m_Textures[BLUE] = IMG_LoadTexture(get_renderer(),
            blueTexturePath);
    if (m_Textures[BLUE] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", blueTexturePath);
    }

    m_Textures[GREEN] = IMG_LoadTexture(get_renderer(),
            greenTexturePath);
    if (m_Textures[GREEN] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", greenTexturePath);
    }

    m_Textures[YELLOW] = IMG_LoadTexture(get_renderer(),
            yellowTexturePath);
    if (m_Textures[YELLOW] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", yellowTexturePath);
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
        if(piece->blocks[i].rect.x + (int)piece->blocks[i].rect.w >= border ||
            is_colliding_piece_blocks_right(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
}

static bool is_colliding_piece_left(const Piece *piece, int border) {
    int i = 0;
    for (; i < 4; ++i) {
        if (piece->blocks[i].rect.x <= border ||
            is_colliding_piece_blocks_left(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
}

static bool is_colliding_piece_bottom(const Piece *piece, int border) {
    int i;
    for (i = 0; i < 4; ++i) {
        if (piece->blocks[i].rect.y + (int)piece->blockWidth >= border ||
            is_colliding_piece_blocks_bottom(&piece->blocks[i])) {
            return true;
        }
    }
    return false;
}

static void player_change_speed(Piece *piece, int *xSpeedP, 
        unsigned int *yTimeStepP) {
    if (is_key_pressed(SDLK_LEFT) && !is_colliding_piece_left(piece, 0)) {
        *xSpeedP = -1;
    } else if(is_key_pressed(SDLK_RIGHT) && 
              !is_colliding_piece_right(piece, get_screen_width())) {
        *xSpeedP = 1;
    } else if(is_key_pressed(SDLK_DOWN)) {
        *yTimeStepP = SMALL_Y_TIME_STEP;
    }
}

void update_piece(Piece *piece) {
    static unsigned int xPrevTicks = 0;
    static unsigned int yPrevTicks = 0;
    unsigned int yTimeStep = BIG_Y_TIME_STEP;
    const unsigned int xTimeStep = X_TIME_STEP;
    int xSpeed = 0;
    player_change_speed(piece, &xSpeed, &yTimeStep);
    unsigned int ticks = SDL_GetTicks();
    if ((int)ticks - xPrevTicks > xTimeStep) {
        int i = 0;
        for (; i < 4; ++i) {
            piece->blocks[i].rect.x += xSpeed * piece->blockWidth;
        }
        if ((int)ticks - yPrevTicks > yTimeStep) {
            bool hit_ground = false;
            for (i = 0; i < 4; ++i) {
                piece->blocks[i].rect.y += piece->blockWidth;
                if (is_colliding_piece_bottom(piece, get_screen_height())) {
                    hit_ground = true;
                }
            }
            if (hit_ground) {
                add_piece(piece);
                *piece = create_piece(random() % 6, piece->x,
                        -2 * piece->blockWidth + piece->blockWidth / 2,
                        piece->blockWidth, random() % 4);
            }
            yPrevTicks = ticks;
        }
        xPrevTicks = ticks;
    }
}
