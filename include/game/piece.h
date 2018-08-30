#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include "block.h"

#define MAX_PIECES 40
#define PIECE_TYPES 7

typedef enum { 
    LBED, LCHAIR, TABLE, RBED, RCHAIR, PILLAR, BLOCK
} PieceType;

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
void update_piece(Piece *piece, Piece *nextPiece, float deltaTime);

bool is_colliding_piece_blocks_left(const Block *block);
bool is_colliding_piece_blocks_right(const Block *block);
bool is_colliding_piece_blocks_bottom(const Block *block);

#endif // GAME_PIECE_H
