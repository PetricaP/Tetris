#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <framework/font_manager.h>
#include <framework/input_manager.h>
#include <framework/list.h>
#include <framework/particle.h>
#include <framework/window.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLOCKS 40
#define MAX_PARTICLES 200

#define MIN_PARTICLE_SPEED_X (-30)
#define MAX_PARTICLE_SPEED_X 30

#define MIN_PARTICLE_SPEED_Y (-70)
#define MAX_PARTICLE_SPEED_Y 20

#define MIN_PARTICLE_RADIUS 1
#define MAX_PARTICLE_RADIUS 4

#define MIN_PARTICLE_LIFETIME 100
#define MAX_PARTICLE_LIFETIME 200

#define PARTICLES_PER_BLOCK 10

#define ROW_SCORE 50

static int m_Score = 0;

static SDL_Color m_GridColor;

static unsigned int m_BlockWidth;
static unsigned int m_Rows = 0;
static unsigned int m_Cols = 0;
static Piece m_ActivePiece;

static Block m_Blocks[MAX_BLOCKS][MAX_BLOCKS / 2] = { { { { 0, 0, 0, 0 }, 0, NULL } } };
static Node *m_Particles = NULL; 
static unsigned int m_GridWidth = 0;


SDL_Texture *m_ScoreTexture = NULL;
SDL_Texture *m_ScoreTextTexture = NULL;

const Block *get_blocks(unsigned int i, unsigned int j) {
	return &m_Blocks[i][j];
}

unsigned int get_grid_width(void) {
    return m_GridWidth;
}

static void update_score() {
    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    SDL_Color color = { 200, 200, 200, 255 };
    m_ScoreTexture = generate_text_texture(scoreString, color);
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

void add_block(Block *block) {
    int col = block->rect.x / block->rect.w;
    int row = block->rect.y / block->rect.h - 1;
    memcpy(&m_Blocks[row][col], block, sizeof(Block));
}

void add_piece(Piece *piece) {
	int i = 0;
	for (; i < 4; ++i) {
        add_block(&piece->blocks[i]);
	}
}

unsigned int get_cols(void) { return m_Cols; }

unsigned int get_rows(void) { return m_Rows; }

void init_graphics(void) {
	/* Initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
}

void init_random(void) {
	srand(time(NULL));
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) {
		fprintf(stderr, "Error while getting time");
	}
	srandom((unsigned)ts.tv_nsec ^ (unsigned)ts.tv_sec); /* Seed the PRNG */
}

void init_game(unsigned int blockWidth) {
    init_random();
    init_fonts();
	m_BlockWidth = blockWidth;
	m_GridWidth = get_screen_width() - 7 * m_BlockWidth;
	m_Rows = get_screen_height() / m_BlockWidth;
	m_Cols = m_GridWidth / m_BlockWidth;
    set_font("res/DejaVu.ttf");
    update_score();

    SDL_Color color = { 100, 100, 100, 255 };
    m_ScoreTextTexture = generate_text_texture("Score", color);
}

void close_game(void) {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void create_initial_piece(void) {
	m_ActivePiece = create_piece(random() % PIECE_TYPES, m_GridWidth / 2,
                    -2.5 * m_BlockWidth, m_BlockWidth, random() % 4);
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
		*gameState = EXIT;
	}
}

void draw_grid() {
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

void draw_particles(void) {
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
    draw_score();
    draw_particles();
}

static vec4u pick_particle_color(BlockColor blockColor) {
    const int enhancement = 150;
    const int baseValue = 55;
    const int salt = 50;
    vec4u color = { baseValue + random() % salt, baseValue + random() % salt,
                    baseValue + random() % salt, 255 };
    switch (blockColor) {
        case RED:
            color.r += random() % enhancement;
            break;
        case BLUE:
            color.b += random() % enhancement;
            break;
        case GREEN:
            color.g += random() % enhancement;
            break;
        case YELLOW:
            color.r += random() % enhancement;
            color.g += random() % enhancement;
            break;
    }
    return color;
}

static void push_particles(Block block) {
    unsigned int i;
    for (i = 0; i < PARTICLES_PER_BLOCK; ++i) {
        vec2f position = { block.rect.x + (float)block.rect.w / 2,
                           block.rect.y + (float)block.rect.h / 2};
        vec2f velocity =
            { (random() % (MAX_PARTICLE_SPEED_X
            - MIN_PARTICLE_SPEED_X) + MIN_PARTICLE_SPEED_X) / 20.0f,
            (random() % (MAX_PARTICLE_SPEED_X
            - MIN_PARTICLE_SPEED_Y) + MIN_PARTICLE_SPEED_Y) / 20.0f };
        vec4u color = pick_particle_color(block.color);
        int radius = 
            random() % (MAX_PARTICLE_RADIUS
            - MIN_PARTICLE_RADIUS) + MIN_PARTICLE_RADIUS;
        int lifeTime = 
            random() % (MAX_PARTICLE_LIFETIME
            - MIN_PARTICLE_LIFETIME) + MIN_PARTICLE_LIFETIME;
        Particle particle = create_particle(position, radius,
                                            lifeTime, velocity,
                                            color);
        insert_node(&m_Particles, particle);
    }
}

static void destroy_rows(unsigned int destroyedRow, 
                         unsigned int numDestroyedRows) {
    unsigned int z, i;
    for (i = 0; i < numDestroyedRows; ++i) {
        unsigned int row = destroyedRow + i;
        for (z = 0; z < get_cols(); ++z) {
            Block block = m_Blocks[row][z];
            push_particles(block);
            m_Blocks[row][z] = m_Blocks[row - numDestroyedRows][z];
            m_Blocks[row][z].rect.y += numDestroyedRows * m_BlockWidth;
        }
    }
    for(i = destroyedRow; i > numDestroyedRows; --i) {
        for (z = 0; z < get_cols(); ++z) {
            m_Blocks[i][z] = m_Blocks[i - numDestroyedRows][z];
            m_Blocks[i][z].rect.y += numDestroyedRows * m_BlockWidth;
        }
    }
    m_Score += numDestroyedRows * ROW_SCORE * pow(2, numDestroyedRows);
    update_score();
}

void update_blocks(void) {
	unsigned int i, j;
	unsigned int rows = get_rows();
	unsigned int cols = get_cols();
    unsigned int numDestroyedRows = 0;
    int destroyedRow = 0;
	for (i = rows; i > 0; --i) {
		unsigned int blocksInRow = 0;
		for (j = 0; j < cols; ++j) {
			Block block = m_Blocks[i][j];
			if (block.m_Texture != NULL) {
				++blocksInRow;
			}
		}
		if (blocksInRow == cols) {
            ++numDestroyedRows;
            destroyedRow = i;
		}
	}
    destroy_rows(destroyedRow, numDestroyedRows);
}

void update_particles(void) {
    if (m_Particles != NULL) {
        if (m_Particles->data.currentLifeTime <= 0 ||
            m_Particles->data.position.x < 0 ||
            m_Particles->data.position.x > get_screen_width() ||
            m_Particles->data.position.y > get_screen_height()) {
            remove_head(&m_Particles);
        } else {
            update_particle(&m_Particles->data);
            Node *node = m_Particles;
            while (node->next != NULL) {
                if(node->next->data.currentLifeTime <= 0 ||
                   node->next->data.position.x < 0 ||
                   node->next->data.position.x > get_screen_width() ||
                   node->next->data.position.y > get_screen_height()) {
                    remove_next_node(node);
                } else {
                    update_particle(&node->next->data);
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

void draw_score(void) {
    vec2i scorePosition = { get_grid_width() +
                            (get_screen_width() - get_grid_width()) / 2,
                            5 * m_BlockWidth };
    int letterWidth = m_BlockWidth * 3 / 4;
    int letterHeight = letterWidth * 5 / 3;

    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    draw_text_texture(m_ScoreTexture, scoreString, scorePosition,
                      letterWidth, letterHeight);
    scorePosition.y -= m_BlockWidth / 2;
    draw_text_texture(m_ScoreTextTexture, "Score", scorePosition,
                      letterWidth, letterHeight);
}
