#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "piece.h"

typedef enum { PLAY, PAUSE, EXIT } GameState;

void init_graphics(void);
void init_game(unsigned int blockWidth);
void close_game();
void cap_fps(unsigned int max_fps);
void process_input(GameState* gameState);
void draw_game();
void update_game();
void update_blocks(void);
unsigned int get_screen_width(void);
unsigned int get_screen_height(void);
const Block *get_blocks(unsigned int i, unsigned int j);
unsigned int get_cols(void);
unsigned int get_rows(void);
void add_piece(Piece *piece);
void add_block(Block *block);
unsigned int get_grid_width(void);
void set_grid_clear_color(unsigned char r, 
        unsigned char g, unsigned char b, unsigned char a);
void draw_grid(void);
void draw_blocks(void);
void draw_particles(void);
void init_random(void);
void create_initial_piece(void);
void update_particles(void);
void draw_score(void);

#endif // GAME_GAME_H

