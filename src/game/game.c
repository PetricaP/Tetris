#include "framework/font_manager.h"
#include "framework/input_manager.h"
#include "framework/list.h"
#include "framework/particle.h"
#include "framework/framework_time.h"
#include "framework/window.h"
#include "game.h"
#include "piece_factory.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLOCKS 40
#define MAX_PARTICLES 200

#define MAX_PARTICLE_SPEED_X 1000

#define MAX_PARTICLE_SPEED_Y 1000

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

static SDL_Texture *m_ScoreTexture = NULL;
static SDL_Texture *m_ScoreTextTexture = NULL;

const Block *get_grid_block(unsigned int i, unsigned int j) {
    return m_Blocks + i * m_Cols + j;
}

unsigned int get_grid_width(void) { return m_GridWidth; }

static vec4u pick_particle_color(BlockColor blockColor);

static void show_particle_shower(vec2i position) {
    int i;
    for (i = 0; i < 15; ++i) {
        vec2f newPosition = {position.x +
                                 (float)m_GridWidth / 3 * (random() % 3 - 1),
                             position.y};

        vec4u color = pick_particle_color(random() % 4);
        vec2f velocity = {(float)(random() % MAX_PARTICLE_SPEED_X -
                                  MAX_PARTICLE_SPEED_X / 2) /
                              MAX_PARTICLE_SPEED_X,
                          (float)(random() % MAX_PARTICLE_SPEED_X -
                                  MAX_PARTICLE_SPEED_X / 2) /
                              MAX_PARTICLE_SPEED_X};
        Particle particle =
            create_particle(newPosition, random() % 5, 120, velocity, color);
        insert_node(&m_Particles, particle);
    }
}

static void show_end_screen(void) {
    static SDL_Texture *endTextTexture = NULL;
    static char message[] = "GAME OVER";
    static char numLetters = sizeof(message) / sizeof(char) - 1;
    if (endTextTexture == NULL) {
        set_font("res/Fuego_Fatuo.ttf");
        SDL_Color color = {190, 190, 190, 255};
        endTextTexture = generate_text_texture("GAME OVER", color);
        set_font("res/DejaVu.ttf");
    }
    int letterWidth = m_BlockWidth * 2;
    int letterHeight = letterWidth * 5 / 3;
    show_particle_shower(m_EndScreenPosition);
    draw_text_texture(endTextTexture, numLetters, m_EndScreenPosition,
                      letterWidth, letterHeight);
}

static void update_score(void) {
    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    SDL_Color scoreColor = {200, 200, 200, 200};
    if (m_ScoreTexture != NULL) {
        SDL_DestroyTexture(m_ScoreTexture);
        m_ScoreTexture = NULL;
    }
    m_ScoreTexture = generate_text_texture(scoreString, scoreColor);
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
    SDL_Color scoreTextColor = {255, 136, 66, 255};
    m_ScoreTextTexture = generate_text_texture("Score", scoreTextColor);
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
    m_ActivePiece =
        create_piece(random() % PIECE_TYPES, m_GridWidth / 2,
                     -2.5 * m_BlockWidth, m_BlockWidth, random() % 4);
    int x = get_grid_width() + 3.5 * m_BlockWidth;
    int y = m_BlockWidth * 10;
    m_NextPiece =
        create_piece(random() % PIECE_TYPES, x, y, m_BlockWidth, random() % 4);
}

void process_input(void) {
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

void draw_grid(void) {
    unsigned int step = m_BlockWidth;
    SDL_Color backgroundColor = get_clear_color();
    SDL_SetRenderDrawColor(get_renderer(), m_GridColor.r, m_GridColor.g,
                           m_GridColor.b, m_GridColor.a);
    const SDL_Rect grid = {0, 0, get_grid_width(), get_screen_height()};
    SDL_RenderFillRect(get_renderer(), &grid);
    SDL_SetRenderDrawColor(get_renderer(), 80, 50, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(get_renderer(), 1, 0, 1, get_screen_height());
    for (; step <= get_grid_width(); step += m_BlockWidth) {
        SDL_RenderDrawLine(get_renderer(), step + 1, 0, step + 1,
                           get_screen_height());
        SDL_RenderDrawLine(get_renderer(), step - 1, 0, step - 1,
                           get_screen_height());
    }
    SDL_RenderDrawLine(get_renderer(), 0, 1, m_GridWidth, 1);
    for (step = m_BlockWidth; step < get_screen_height();
         step += m_BlockWidth) {
        SDL_RenderDrawLine(get_renderer(), 0, step + 1, m_GridWidth, step + 1);
        SDL_RenderDrawLine(get_renderer(), 0, step - 1, m_GridWidth, step - 1);
    }
    SDL_RenderDrawLine(get_renderer(), 0, get_screen_height(), m_GridWidth,
                       get_screen_height());
    SDL_SetRenderDrawColor(get_renderer(), backgroundColor.r, backgroundColor.g,
                           backgroundColor.b, backgroundColor.a);
}

void draw_particles(void) {
    Node *node = m_Particles;
    while (node) {
        draw_particle(&node->data);
        node = node->next;
    }
}

static void show_boom(void);
static bool m_Boom = false;

void draw_game(void) {
    draw_grid();
    draw_piece(&m_ActivePiece);
    draw_piece(&m_NextPiece);
    draw_blocks();
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
    vec4u color = {baseValue + random() % salt, baseValue + random() % salt,
                   baseValue + random() % salt, 255};
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
        vec2f position = {x, y};
        vec2f velocity = {(float)(random() % MAX_PARTICLE_SPEED_X -
                                  MAX_PARTICLE_SPEED_X / 2) /
                              MAX_PARTICLE_SPEED_X,
                          (float)(random() % MAX_PARTICLE_SPEED_X -
                                  MAX_PARTICLE_SPEED_X / 2) /
                              MAX_PARTICLE_SPEED_X};
        int radius = random() % (MAX_PARTICLE_RADIUS - MIN_PARTICLE_RADIUS) +
                     MIN_PARTICLE_RADIUS;
        int lifeTime =
            random() % (MAX_PARTICLE_LIFETIME - MIN_PARTICLE_LIFETIME) +
            MIN_PARTICLE_LIFETIME;
        Particle particle =
            create_particle(position, radius, lifeTime, velocity, color);
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
            push_particles(block.rect.x + block.rect.w,
                           block.rect.y + block.rect.h,
                           pick_particle_color(block.color));
            m_Blocks[row * m_Cols + z] =
                m_Blocks[(row - numDestroyedRows) * m_Cols + z];
            m_Blocks[row * m_Cols + z].rect.y +=
                numDestroyedRows * m_BlockWidth;
        }
    }
    for (i = destroyedRow; i > numDestroyedRows; --i) {
        for (z = 0; z < m_Cols; ++z) {
            m_Blocks[i * m_Cols + z] =
                m_Blocks[(i - numDestroyedRows) * m_Cols + z];
            m_Blocks[i * m_Cols + z].rect.y += numDestroyedRows * m_BlockWidth;
        }
    }
    m_Score += numDestroyedRows * ROW_SCORE * pow(2, numDestroyedRows);
    update_score();
}

static void show_boom(void) {
    static SDL_Texture *boomTextTexture = NULL;
    static char message[] = "BOOM! Tetris for JEFF!";
    static int numLetters = sizeof(message) / sizeof(char) - 1;
    if (boomTextTexture == NULL) {
        set_font("res/Fuego_Fatuo.ttf");
        SDL_Color color = {205, 102, 82, SDL_ALPHA_OPAQUE};
        boomTextTexture = generate_text_texture(message, color);
    }
    set_font("res/DejaVu.ttf");
    int letterWidth = m_BlockWidth * 3 / 5;
    int letterHeight = letterWidth * 5 / 3;
    vec2i boomPosition = {m_EndScreenPosition.x - 100, m_EndScreenPosition.y};
    show_particle_shower(boomPosition);
    draw_text_texture(boomTextTexture, numLetters, boomPosition, letterWidth,
                      letterHeight);
}

static void check_boom(int numRows, float deltaTime) {
    static int counter = 0;
    if (numRows == 4) {
        counter = 5 * deltaTime;
    }
    if (counter > 0) {
        m_Boom = true;
    } else {
        m_Boom = false;
    }
    --counter;
}

void update_blocks(float deltaTime) {
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
    check_boom(numDestroyedRows, deltaTime);
}

void update_particles(float deltaTime) {
    if (m_Particles != NULL) {
        if (m_Particles->data.currentLifeTime <= 0 ||
            m_Particles->data.position.x < 0 ||
            m_Particles->data.position.x > get_screen_width() ||
            m_Particles->data.position.y > get_screen_height()) {
            remove_head(&m_Particles);
        } else {
            update_particle(&m_Particles->data, deltaTime);
            Node *node = m_Particles;
            while (node->next != NULL) {
                if (node->next->data.currentLifeTime <= 0 ||
                    node->next->data.position.x < 0 ||
                    node->next->data.position.x > get_screen_width() ||
                    node->next->data.position.y > get_screen_height()) {
                    remove_next_node(node);
                } else {
                    update_particle(&node->next->data, deltaTime);
                    node = node->next;
                }
            }
        }
    }
}

void set_grid_clear_color(unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a) {
    m_GridColor.r = r;
    m_GridColor.g = g;
    m_GridColor.b = b;
    m_GridColor.a = a;
}

void update_game(float deltaTime) {
    update_piece(&m_ActivePiece, &m_NextPiece, deltaTime);
    update_blocks(deltaTime);
    update_particles(deltaTime);
}

void draw_score(void) {
    vec2i scorePosition = {get_grid_width() +
                               (get_screen_width() - get_grid_width()) / 2,
                           5 * m_BlockWidth};
    int letterWidth = m_BlockWidth * 1 / 2;
    int letterHeight = letterWidth * 5 / 3;

    char scoreString[12];
    sprintf(scoreString, "%d", m_Score);
    draw_text_texture(m_ScoreTexture, strlen(scoreString), scorePosition,
                      letterWidth, letterHeight);
    scorePosition.y -= 3 * m_BlockWidth / 2;
    draw_text_texture(m_ScoreTextTexture, 5, scorePosition, letterWidth * 4 / 3,
                      letterHeight * 4 / 3);
}
