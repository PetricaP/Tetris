#include "framework/framework_time.h"
#include "game/game.h"
#include "game/piece_factory.h"
#include <SDL2/SDL_image.h>
#include <framework/window.h>
#include <stdlib.h>

static Window m_Window;

const unsigned int blockWidth = 30;
const unsigned int screenWidth = 660;
const unsigned int screenHeight = 810;

void init_test(unsigned int numRows, int offset) {
    numRows += offset;
    int rand = BLUE;
    Block block = create_block(get_texture(rand), rand, 5 * blockWidth,
                               get_screen_height() - blockWidth * (numRows + 2),
                               blockWidth);
    add_block(&block);
    rand = BLUE;
    block = create_block(get_texture(rand), rand, 4 * blockWidth,
                         get_screen_height() - blockWidth * (numRows + 2),
                         blockWidth);
    add_block(&block);
    rand = RED;
    block = create_block(get_texture(rand), rand, 4 * blockWidth,
                         get_screen_height() - blockWidth * (numRows + 1),
                         blockWidth);
    add_block(&block);
    rand = GREEN;
    block = create_block(get_texture(rand), rand, 3 * blockWidth,
                         get_screen_height() - blockWidth * (numRows + 1),
                         blockWidth);
    add_block(&block);
    rand = GREEN;
    block = create_block(get_texture(rand), rand, 3 * blockWidth,
                         get_screen_height() - blockWidth * (numRows + 3),
                         blockWidth);
    add_block(&block);
    rand = RED;
    block = create_block(get_texture(rand), rand, 3 * blockWidth,
                         get_screen_height() - blockWidth * (numRows + 4),
                         blockWidth);
    add_block(&block);
    unsigned int i, j;
    for (i = 0; i < get_cols(); ++i) {
        for (j = 1 + offset; j < numRows + 1; ++j) {
            rand = random() % 4;
            block =
                create_block(get_texture(rand), rand, i * blockWidth,
                             get_screen_height() - blockWidth * j, blockWidth);
            add_block(&block);
        }
    }
}

void update_test(float deltaTime) {
    update_particles(deltaTime);
    update_blocks(deltaTime);
}

int main(int argc, char *argv[]) {
    char *err;
    GameState gameState = PLAY;
    init_graphics();
    m_Window = create_window("Hello", screenWidth, screenHeight);
    set_window_clear_color(20, 20, 40, 200);
    set_grid_clear_color(40, 20, 20, 200);
    set_texture_source("./res/red_block.png", "res/blue_block.png",
                       "./res/green_block.png", "res/yellow_block.png");
    init_game(blockWidth, &gameState);
    switch (argc) {
    case 1:
        init_test(4, 0);
        break;
    case 2:
        init_test(strtol(argv[1], &err, 10), 0);
        break;
    default:
        init_test(strtol(argv[1], &err, 10), strtol(argv[2], &err, 10));
    }
    unsigned int ticks = 0;
    unsigned int prevTicks = 0;
    while (gameState != EXIT) {
        float deltaTime = ticks - prevTicks;
        prevTicks = ticks;
        process_input();
        if (gameState != PAUSE) {
            update_test(deltaTime);
        }
        draw_game();
        cap_fps(60);
        clear_window();
        ticks = get_ticks(); 
    }
    delete_window(&m_Window);
    close_game();
    return 0;
}
