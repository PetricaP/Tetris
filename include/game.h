#ifndef GAME_H_
#define GAME_H_

#include "piece.h"

typedef enum { PLAY, PAUSE, EXIT } GameState;

void init_graphics(void);
void init_game(unsigned int blockWidth);
void close_game();
void cap_fps(unsigned int max_fps);
void process_input(GameState* gameState);
void draw_game();
void update_game();
unsigned int get_screen_width(void);
unsigned int get_screen_height(void);
const Block *get_blocks(unsigned int i, unsigned int j);
unsigned int get_cols(void);
unsigned int get_rows(void);
void add_piece(Piece *piece);

#endif
