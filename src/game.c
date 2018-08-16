#include "game.h"
#include "input_manager.h"
#include "list.h"
#include "particle.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLOCKS 40
#define MAX_PARTICLES 200

static SDL_Color m_GridColor;

static unsigned int m_BlockWidth;
static unsigned int m_Rows = 0;
static unsigned int m_Cols = 0;
static Piece m_ActivePiece;
static Block m_Blocks[MAX_BLOCKS][MAX_BLOCKS / 2] = {{{{0, 0, 0, 0}, 0, NULL}}};
static Node *m_Particles = NULL; 
static unsigned int m_GridWidth;

const Block *get_blocks(unsigned int i, unsigned int j) {
	return &m_Blocks[i][j];
}

unsigned int get_grid_width(void) {
    return m_GridWidth;
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

unsigned int get_cols(void) { return m_Cols; }

unsigned int get_rows(void) { return m_Rows; }

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
	srandom((unsigned)ts.tv_nsec ^ (unsigned)ts.tv_sec); /* Seed the PRNG */

	m_BlockWidth = blockWidth;
	m_GridWidth = get_screen_width() - 7 * m_BlockWidth;
	m_Rows = get_screen_height() / m_BlockWidth;
	m_Cols = m_GridWidth / m_BlockWidth;
	m_ActivePiece = create_piece(random() % 6, m_GridWidth / 2,
                    -2.5 * m_BlockWidth, m_BlockWidth, random() % 4);
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

void process_input(GameState *const gameState) {
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
    SDL_SetRenderDrawColor(get_renderer(), m_GridColor.r, m_GridColor.g,
                           m_GridColor.b, m_GridColor.a);
    const SDL_Rect grid = { 0, 0, get_grid_width(), get_screen_height() };
    SDL_RenderFillRect(get_renderer(), &grid);
	SDL_SetRenderDrawColor(get_renderer(), 80, 50, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(get_renderer(), 1, 0, 1, get_screen_height());
	for (; step <= get_grid_width(); step += m_BlockWidth) {
		SDL_RenderDrawLine(get_renderer(), step + 1, 0, step + 1, get_screen_height());
		SDL_RenderDrawLine(get_renderer(), step - 1, 0, step - 1, get_screen_height());
	}
    SDL_RenderDrawLine(get_renderer(), 0, 1, m_GridWidth, 1);
	for (step = m_BlockWidth; step < get_screen_height(); step += m_BlockWidth) {
		SDL_RenderDrawLine(get_renderer(), 0, step + 1, m_GridWidth, step + 1);
		SDL_RenderDrawLine(get_renderer(), 0, step - 1, m_GridWidth, step - 1);
	}
    SDL_RenderDrawLine(get_renderer(), 0, get_screen_height(), m_GridWidth,
                       get_screen_height());
	SDL_SetRenderDrawColor(get_renderer(), backgroundColor.r,
                           backgroundColor.g, backgroundColor.b,
                           backgroundColor.a);
}

static void draw_particles(void) {
    Node *node = m_Particles;
    while (node) {
        draw_particle(&node->data);
        node = node->next;
    }
}

void draw_game() {
	draw_grid();
	draw_piece(&m_ActivePiece);
	draw_blocks();
    draw_particles();
}

static vec4u pick_particle_color(BlockColor blockColor) {
    vec4u color = {10 + random() % 20, 10 + random() % 20, 10 + random() % 20, 255};
    switch (blockColor) {
        case RED:
            color.r += 70 + random() % 100;
            break;
        case BLUE:
            color.b += 70 + random() % 100;
            break;
        case GREEN:
            color.g += 70 + random() % 100;
            break;
        case YELLOW:
            color.r += 70 + random() % 100;
            color.g += 70 + random() % 100;
            break;
    }
    return color;
}

static void destroy_row(unsigned int row) {
    unsigned int k, z, i;
    for (k = row; k > 0; --k) {
        for (z = 0; z < get_cols(); ++z) {
            Block block = m_Blocks[k][z];
            if(k == row) {
                for (i = 0; i < 10; ++i) {
                    vec2f position = { block.rect.x, block.rect.y };
                    vec2f velocity = { (random() % 100 - 50) / 25.0f,
                                       (random() % 100 - 50) / 25.0f};
                    vec4u color = pick_particle_color(block.color);
                    int radius = random() % 3 + 2;
                    int lifeTime = random() % 200 + 200;
                    Particle particle = create_particle(position, radius,
                                                        lifeTime, velocity,
                                                        color);
                    insert_node(&m_Particles, particle);
                }
            }
            m_Blocks[k][z] = m_Blocks[k - 1][z];
            m_Blocks[k][z].rect.y += m_BlockWidth;
        }
    }
}

static void update_blocks(void) {
	unsigned int i, j;
	unsigned int rows = get_rows();
	unsigned int cols = get_cols();
    unsigned int numDestroyedRows = 0;
    int destroyedRows[4] = {0};
	for (i = rows; i > 0; --i) {
		unsigned int blocksInRow = 0;
		for (j = 0; j < cols; ++j) {
			Block block = m_Blocks[i][j];
			if (block.m_Texture != NULL) {
				++blocksInRow;
			}
		}
		if (blocksInRow == cols) {
            destroyedRows[numDestroyedRows++] = i;
		}
	}
    for (i = 0; i < numDestroyedRows; ++i) {
        destroy_row(destroyedRows[i]);
    }
}

static void update_particles(void) {
    if (m_Particles != NULL) {
        if (!m_Particles->next && m_Particles->data.currentLifeTime == 0) {
            remove_head(&m_Particles);
        } else {
            Node *node = m_Particles;
            while (node->next != NULL) {
                if(node->next->data.currentLifeTime == 0) {
                    remove_next_node(node);
                } else {
                    update_particle(&node->data);
                    node = node->next;
                }
            }
        }
    }
}

void set_grid_clear_color(unsigned char r, 
        unsigned char g, unsigned char b, unsigned char a) {
    m_GridColor.r = r;
    m_GridColor.g = g;
    m_GridColor.b = b;
    m_GridColor.a = a;
}

void update_game(void) {
	update_piece(&m_ActivePiece);
	update_blocks();
    update_particles();
}
