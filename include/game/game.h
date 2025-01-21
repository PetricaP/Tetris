#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "piece.h"

typedef enum { PLAY = 0, PAUSE = 1, EXIT = 2, END = 3 } GameState;
void init_graphics(void);
void init_game(unsigned int blockWidth, GameState *gameState);
void close_game(void);
void process_input(void);
void draw_game(void);
void update_game(float deltaTime);
void update_blocks(float deltaTime);
unsigned int get_screen_width(void);
unsigned int get_screen_height(void);
const Block *get_grid_block(unsigned int i, unsigned int j);
unsigned int get_cols(void);
unsigned int get_rows(void);
void add_piece(Piece *piece);
void add_block(Block *block);
unsigned int get_grid_width(void);
void set_grid_clear_color(unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a);
void draw_grid(void);
void draw_blocks(void);
void draw_particles(void);
void init_random(void);
void create_initial_pieces(void);
void update_particles(float deltaTime);
void draw_score(void);

#endif // GAME_GAME_H
