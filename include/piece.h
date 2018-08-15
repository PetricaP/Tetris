#ifndef PIECE_H_
#define PIECE_H_

#include "block.h"

#define MAX_PIECES 40

typedef enum { 
    LBED, LCHAIR, TABLE, RBED, RCHAIR, PILLAR
} PieceType;

typedef enum {
    RED = 0, BLUE = 1, GREEN = 2, YELLOW = 3
} BlockColor;

typedef struct {
    int x, y;
    unsigned int blockWidth;
    PieceType type;
    BlockColor color;
    Block blocks[4];
    SDL_Texture *m_BlockTexture;
} Piece;

Piece create_piece(PieceType type, int x, int y, unsigned int blockWidth,
        BlockColor color);
void draw_piece(const Piece *piece);
void update_piece(Piece *piece);
void set_texture_source(const char *redTexturePath, 
                        const char *blueTexturePath,
                        const char *greenTexturePath,
                        const char *yellowTexturePath);

bool is_colliding_piece_blocks_left(const Block *block);
bool is_colliding_piece_blocks_right(const Block *block);
bool is_colliding_piece_blocks_bottom(const Block *block);

#endif
