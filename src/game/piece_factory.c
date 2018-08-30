#include "block.h"
#include "framework/window.h"
#include "piece_factory.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static SDL_Texture *m_Textures[4] = {NULL};

static void initialize_piece_center(Piece *piece);
static void initialize_piece_bed(Piece *piece);
static void initialize_piece_lbed(Piece *piece);
static void initialize_piece_rbed(Piece *piece);
static void initialize_piece_lchair(Piece *piece);
static void initialize_piece_rchair(Piece *piece);
static void initialize_piece_pillar(Piece *piece);
static void initialize_piece_table(Piece *piece);
static void initialize_piece_block(Piece *piece);

SDL_Texture *get_texture(BlockColor color) { return m_Textures[(int)color]; }

void initialize_piece(Piece *piece) {
    switch (piece->type) {
    case LBED:
        initialize_piece_lbed(piece);
        break;

    case RBED:
        initialize_piece_rbed(piece);
        break;

    case TABLE:
        initialize_piece_table(piece);
        break;

    case LCHAIR:
        initialize_piece_lchair(piece);
        break;

    case RCHAIR:
        initialize_piece_rchair(piece);
        break;

    case PILLAR:
        initialize_piece_pillar(piece);
        break;

    case BLOCK:
        initialize_piece_block(piece);
        break;
    }
}

static void initialize_piece_block(Piece *piece) {
    piece->blocks[0] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);
    piece->blocks[1] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);
    piece->blocks[2] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y + piece->blockWidth / 2, piece->blockWidth);
    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y + piece->blockWidth / 2, piece->blockWidth);
}

/*  000  */
/*  010  */
/*  000  */
static void initialize_piece_center(Piece *piece) {
    piece->blocks[0] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);
}

/*  000  */
/*  111  */
/*  000  */
static void initialize_piece_bed(Piece *piece) {
    initialize_piece_center(piece);

    piece->blocks[1] =
        create_block(piece->m_BlockTexture, piece->color,
                     piece->x - piece->blockWidth * 3 / 2,
                     piece->y - piece->blockWidth / 2, piece->blockWidth);

    piece->blocks[2] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);
}

/*  100  */
/*  111  */
/*  000  */
static void initialize_piece_lbed(Piece *piece) {
    initialize_piece_bed(piece);

    piece->blocks[3] =
        create_block(piece->m_BlockTexture, piece->color,
                     piece->x - piece->blockWidth * 3 / 2,
                     piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);
}

/*  001  */
/*  111  */
/*  000  */
static void initialize_piece_rbed(Piece *piece) {
    initialize_piece_bed(piece);

    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);
}

/*  010  */
/*  111  */
/*  000  */
static void initialize_piece_table(Piece *piece) {
    initialize_piece_bed(piece);

    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);
}

/*  001  */
/*  011  */
/*  010  */
static void initialize_piece_rchair(Piece *piece) {
    initialize_piece_center(piece);

    piece->blocks[1] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);

    piece->blocks[2] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);

    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y + piece->blockWidth / 2, piece->blockWidth);
}

/*  100  */
/*  110  */
/*  010  */
static void initialize_piece_lchair(Piece *piece) {
    initialize_piece_center(piece);

    piece->blocks[1] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y - piece->blockWidth / 2, piece->blockWidth);

    piece->blocks[2] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);

    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x + piece->blockWidth / 2,
        piece->y + piece->blockWidth / 2, piece->blockWidth);
}

/*  010  */
/*  010  */
/*  010  */
/*  010  */
static void initialize_piece_pillar(Piece *piece) {
    initialize_piece_center(piece);

    piece->blocks[1] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y + piece->blockWidth / 2, piece->blockWidth);

    piece->blocks[2] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth * 3 / 2, piece->blockWidth);

    piece->blocks[3] = create_block(
        piece->m_BlockTexture, piece->color, piece->x - piece->blockWidth / 2,
        piece->y - piece->blockWidth * 5 / 2, piece->blockWidth);
}

void set_texture_source(const char *redTexturePath, const char *blueTexturePath,
                        const char *greenTexturePath,
                        const char *yellowTexturePath) {

    m_Textures[RED] = IMG_LoadTexture(get_renderer(), redTexturePath);
    if (m_Textures[RED] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", redTexturePath);
    }

    m_Textures[BLUE] = IMG_LoadTexture(get_renderer(), blueTexturePath);
    if (m_Textures[BLUE] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", blueTexturePath);
    }

    m_Textures[GREEN] = IMG_LoadTexture(get_renderer(), greenTexturePath);
    if (m_Textures[GREEN] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", greenTexturePath);
    }

    m_Textures[YELLOW] = IMG_LoadTexture(get_renderer(), yellowTexturePath);
    if (m_Textures[YELLOW] == NULL) {
        fprintf(stderr, "Couldn't load texture %s\n", yellowTexturePath);
    }
}
