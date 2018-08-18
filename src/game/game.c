#include "game.h"
#include "piece_factory.h"
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
static Piece m_NextPiece;

static Block *m_Blocks = NULL;
static Node *m_Particles = NULL; 
static unsigned int m_GridWidth = 0;
static GameState *m_GameState;
static vec2i m_EndScreenPosition = {0, 0};

SDL_Texture *m_ScoreTexture = NULL;
SDL_Texture *m_ScoreTextTexture = NULL;

const Block *get_blocks(unsigned int i, unsigned int j) {
	return m_Blocks + i * m_Cols + j;
}

unsigned int get_grid_width(void) {
    return m_GridWidth;
}

static vec4u pick_particle_color(BlockColor blockColor);

static void show_particle_shower(vec2i position) {
    int i;
    for (i = 0; i < 5; ++i) {
        vec2f newPosition = { position.x +
                              (float)(random() % get_screen_width()) * 4 / 5
                              - (float)get_screen_width() * 2 / 5, position.y };

        vec4u color = pick_particle_color(random() % 4);
        vec2f velocity = { random() % 4 - 2, random() % 6 - 5};
        Particle particle = create_particle(newPosition, random() % 5,
                                            120, velocity,
                                            color);
        insert_node(&m_Particles, particle);
    }
}

static void show_end_screen() {
    static SDL_Texture *endTextTexture = NULL;
    if (endTextTexture == NULL) {
        set_font("res/Fuego_Fatuo.ttf");
        SDL_Color color = { 130, 220, 140, 255 };
        endTextTexture = generate_text_texture("GAME OVER", color);
        set_font("res/DejaVu.ttf");
    }
    int letterWidth = m_BlockWidth * 2;
    int letterHeight = letterWidth * 5 / 3;
    show_particle_shower(m_EndScreenPosition);
    draw_text_texture(endTextTexture, "GAME OVER", m_EndScreenPosition,
                      letterWidth, letterHeight);
}

static void update_score() {
    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    SDL_Color color = { 200, 200, 200, 255 };
    if (m_ScoreTexture != NULL) {
        SDL_DestroyTexture(m_ScoreTexture);
        m_ScoreTexture = NULL;
    }
    m_ScoreTexture = generate_text_texture(scoreString, color);
    if (m_ScoreTexture != NULL) {
        SDL_DestroyTexture(m_ScoreTextTexture);
        m_ScoreTextTexture = NULL;
    }
    m_ScoreTextTexture = generate_text_texture("Score", color);
}

void draw_blocks(void) {
	int i;
    int numBlocks = m_Cols * m_Rows;
	for (i = numBlocks - 1; i >= 0; --i) {
        Block block = m_Blocks[i];
        if (block.m_Texture != NULL) {
            draw_block(&block);
        }
	}
}

void add_block(Block *block) {
    int col = block->rect.x / block->rect.w;
    int row = block->rect.y / block->rect.h - 1;
    if (row < 1) {
        *m_GameState = END;
    } else {
        memcpy(&m_Blocks[row * m_Cols + col], block, sizeof(Block));
    }
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

void init_game(unsigned int blockWidth, GameState *gameState) {
    init_random();
    init_fonts();
    m_GameState = gameState;
	m_BlockWidth = blockWidth;
	m_GridWidth = get_screen_width() - 7 * m_BlockWidth;
	m_Rows = get_screen_height() / m_BlockWidth;
	m_Cols = m_GridWidth / m_BlockWidth;
    m_Blocks = (Block *)malloc(m_Rows * m_Cols * sizeof(Block));
    if (m_Blocks == NULL) {
        fprintf(stderr, "Couldn't allocate block memory");
        exit(1);
    }
    m_EndScreenPosition.x = get_screen_width() / 2;
    m_EndScreenPosition.y = get_screen_height() / 3;
    memset(m_Blocks, 0, m_Rows * m_Cols * sizeof(Block));
    set_font("res/DejaVu.ttf");
    update_score();
    set_texture_source("./res/red_block.png", "res/blue_block.png",
                       "./res/green_block.png", "res/yellow_block.png");
    SDL_Color color = { 200, 200, 200, 255 };
    m_ScoreTextTexture = generate_text_texture("Score", color);
}

void close_game(void) {
    free(m_Blocks);
    m_Blocks = NULL;
    SDL_DestroyTexture(m_ScoreTexture);
    m_ScoreTexture = NULL;
    SDL_DestroyTexture(m_ScoreTextTexture);
    m_ScoreTextTexture = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void create_initial_pieces(void) {
	m_ActivePiece = create_piece(random() % PIECE_TYPES, m_GridWidth / 2,
                    -2.5 * m_BlockWidth, m_BlockWidth, random() % 4);
	m_NextPiece = create_piece(random() % PIECE_TYPES, m_GridWidth + 90,
                    m_BlockWidth * 10, m_BlockWidth, random() % 4);
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

void process_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			*m_GameState = EXIT;
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
    static bool canChangeGameState = true;
    if (!is_key_pressed(SDLK_p)) {
        canChangeGameState = true;
    }
	if (is_key_pressed(SDLK_p) && canChangeGameState) {
		*m_GameState = (unsigned int)*m_GameState ^ 1u;
        canChangeGameState = false;
	}
	if (is_key_pressed(SDLK_ESCAPE)) {
		*m_GameState = EXIT;
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

static void show_boom();
static bool m_Boom = false;

void draw_game() {
	draw_grid();
	draw_piece(&m_ActivePiece);
	draw_piece(&m_NextPiece);
	draw_blocks();
    update_particles();
    draw_particles();
    draw_score();
    if (*m_GameState == END) {
        show_end_screen();
    }
    if (m_Boom) {
        show_boom();
    }
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

static void push_particles(float x, float y, vec4u color) {
    unsigned int i;
    for (i = 0; i < PARTICLES_PER_BLOCK; ++i) {
        vec2f position = { x, y };
        vec2f velocity =
            { (random() % (MAX_PARTICLE_SPEED_X
            - MIN_PARTICLE_SPEED_X) + MIN_PARTICLE_SPEED_X) / 20.0f,
            (random() % (MAX_PARTICLE_SPEED_X
            - MIN_PARTICLE_SPEED_Y) + MIN_PARTICLE_SPEED_Y) / 20.0f };
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
        for (z = 0; z < m_Cols; ++z) {
            Block block = m_Blocks[row * m_Cols + z];
            push_particles(block.rect.x + block.rect.w, block.rect.y + block.rect.h,
                           pick_particle_color(block.color));
            m_Blocks[row * m_Cols + z] = m_Blocks[(row - numDestroyedRows) * m_Cols + z];
            m_Blocks[row * m_Cols + z].rect.y += numDestroyedRows * m_BlockWidth;
        }
    }
    for(i = destroyedRow; i > numDestroyedRows; --i) {
        for (z = 0; z < m_Cols; ++z) {
            m_Blocks[i * m_Cols + z] = m_Blocks[(i - numDestroyedRows) * m_Cols + z];
            m_Blocks[i * m_Cols + z].rect.y += numDestroyedRows * m_BlockWidth;
        }
    }
    m_Score += numDestroyedRows * ROW_SCORE * pow(2, numDestroyedRows);
    update_score();
}

static void show_boom() {
    static SDL_Texture *boomTextTexture = NULL;
    set_font("res/Fuego_Fatuo.ttf");
    SDL_Color color = { 180, 235, 190, SDL_ALPHA_OPAQUE };
    if (boomTextTexture == NULL) {
        boomTextTexture = generate_text_texture("BOOM! Tetris for JEFF!", color);
    }
    set_font("res/DejaVu.ttf");
    int letterWidth = m_BlockWidth * 3 / 2;
    int letterHeight = letterWidth * 5 / 3;
    vec2i boomPosition = { m_EndScreenPosition.x - 100, m_EndScreenPosition.y};
    show_particle_shower(boomPosition);
    draw_text_texture(boomTextTexture, "GAME OVER", boomPosition,
                      letterWidth, letterHeight);
}

static void check_boom(int numRows) {
    static int counter = 0;
    if (numRows == 4) {
        counter = 90;
    }
    if (counter > 0) {
        m_Boom = true;
    } else {
        m_Boom = false;
    }
    --counter;
}

void update_blocks(void) {
	int i;
    unsigned int numDestroyedRows = 0;
    int numBlocks = m_Rows * m_Cols;
    unsigned int count = 0;
    int destroyedRow = 0;
    bool fullRow = true;
	for (i = numBlocks - 1; i >= 0; --i) {
        ++count;
        Block block = m_Blocks[i];
        if (block.m_Texture == NULL) {
            fullRow = false;
        }
        if (count == m_Cols) {
            if (fullRow) {
                ++numDestroyedRows;
                destroyedRow = i / m_Cols;
            }
            fullRow = true;
            count = 0;
        }
	}
    destroy_rows(destroyedRow, numDestroyedRows);
    check_boom(numDestroyedRows);
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
	update_piece(&m_ActivePiece, &m_NextPiece);
	update_blocks();
}

void draw_score(void) {
    vec2i scorePosition = { get_grid_width() +
                            (get_screen_width() - get_grid_width()) / 2,
                            5 * m_BlockWidth };
    int letterWidth = m_BlockWidth * 1 / 2;
    int letterHeight = letterWidth * 5 / 3;

    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    draw_text_texture(m_ScoreTexture, scoreString, scorePosition,
                      letterWidth, letterHeight);
    scorePosition.y -= m_BlockWidth;
    draw_text_texture(m_ScoreTextTexture, "Score", scorePosition,
                      letterWidth, letterHeight);
}
