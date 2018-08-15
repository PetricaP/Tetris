#include "game.h"
#include "input_manager.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLOCKS 40

static unsigned int m_BlockWidth;
static unsigned int m_Rows = 0;
static unsigned int m_Cols = 0;
static Piece m_ActivePiece;
static Block m_Blocks[MAX_BLOCKS][MAX_BLOCKS / 2] = {{{{0, 0, 0, 0}, NULL}}};

const Block *get_blocks(unsigned int i, unsigned int j) {
    return &m_Blocks[i][j];
}

void draw_blocks(void) {
    unsigned int i, j;
    for (i = get_rows(); i > 0; --i) {
        for (j = 0; j < get_cols(); ++j) {
            Block block = m_Blocks[i][j];
            if (block.m_Texture != NULL) {
                draw_block(&block);
            }
        }
    }
}

void add_piece(Piece *piece) {
    int i = 0;
    for (; i < 4; ++i) {
        Block block = piece->blocks[i];
        int col = block.rect.x / block.rect.w;
        int row = block.rect.y / block.rect.h - 1;
        memcpy(&m_Blocks[row][col], &block, sizeof(Block));
    }
}

unsigned int get_cols(void) {
    return m_Cols;
}

unsigned int get_rows(void) {
    return m_Rows;
}

void init_graphics(void) {
    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
}

void init_game(unsigned int blockWidth) {
    /* Initialzie random number generator */
    srand(time(NULL));
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        fprintf(stderr, "Error while getting time");
    }
    srandom((unsigned)ts.tv_nsec ^ (unsigned)ts.tv_sec);  /* Seed the PRNG */

    m_BlockWidth = blockWidth;
    m_ActivePiece = create_piece(random() % 6, 225, -75, m_BlockWidth, random() % 4);
    m_Rows = get_screen_height() / m_BlockWidth;
    m_Cols = get_screen_width() / m_BlockWidth;
}

void close_game() {
    /* Deinitialize SDL */
    IMG_Quit();
    SDL_Quit();
}

void cap_fps(unsigned int max_fps) {
    static unsigned int prevTicks = 0;
    unsigned int diff = SDL_GetTicks() - prevTicks;
    unsigned int wait = (1.0f / max_fps * 1000);
    if (diff < wait) {
        SDL_Delay(wait - diff);
    }
    prevTicks = SDL_GetTicks();
}

void process_input(GameState * const gameState) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *gameState = EXIT;
                break;
            case SDL_KEYDOWN:
                press_key(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                release_key(event.key.keysym.sym);
                break;
            default:
                break;
        }
    }
    if (is_key_pressed(SDLK_p)) {
        *gameState = PAUSE;
    }
    if (is_key_pressed(SDLK_s)) {
        *gameState = PLAY;
    }
    if (is_key_pressed(SDLK_ESCAPE)) {
        *gameState = PLAY;
    }
}

static void draw_grid() {
    unsigned int step = m_BlockWidth;
    SDL_Color backgroundColor = get_clear_color();
    SDL_SetRenderDrawColor(get_renderer(), 30, 10, 10, SDL_ALPHA_OPAQUE);
    for(; step < get_screen_width(); step += m_BlockWidth) {
        SDL_RenderDrawLine(get_renderer(), step, 0, step, get_screen_height());
    }
    for(step = m_BlockWidth; step < get_screen_height(); step += m_BlockWidth) {
        SDL_RenderDrawLine(get_renderer(), 0, step, get_screen_width(), step);
    }
    SDL_SetRenderDrawColor(get_renderer(), backgroundColor.r, backgroundColor.g,
            backgroundColor.b, backgroundColor.a);
}

void draw_game() {
    draw_grid();
    draw_piece(&m_ActivePiece);
    draw_blocks();
}

void update_game() {
    update_piece(&m_ActivePiece);
}

