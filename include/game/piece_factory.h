#ifndef GAME_PIECE_FACTORY_H
#define GAME_PIECE_FACTORY_H

#include "piece.h"

void initialize_piece(Piece *piece);
SDL_Texture *get_texture(BlockColor color);
void set_texture_source(const char *redTexturePath, 
                        const char *blueTexturePath,
                        const char *greenTexturePath,
                        const char *yellowTexturePath);

#endif // GAME_PIECE_FACTORY_H
