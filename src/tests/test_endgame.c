#include "framework/time.h"
#include "game/game.h"
#include "game/piece_factory.h"
#include <SDL2/SDL_image.h>
#include <framework/window.h>
#include <stdlib.h>

static Window m_Window;

const unsigned int blockWidth = 30;
const unsigned int screenWidth = 660;
const unsigned int screenHeight = 810;

void init_test() {
    int rand = BLUE;
    Block block =
        create_block(get_texture(rand), rand, 5 * blockWidth, 0, blockWidth);
    add_block(&block);
}

void update_test(float deltaTime) {
    update_particles(deltaTime);
    update_blocks(deltaTime);
}

int main(void) {
    GameState gameState = PLAY;
    init_graphics();
    m_Window = create_window("Hello", screenWidth, screenHeight);
    set_window_clear_color(20, 20, 40, 200);
    set_grid_clear_color(40, 20, 20, 200);
    set_texture_source("./res/red_block.png", "res/blue_block.png",
                       "./res/green_block.png", "res/yellow_block.png");
    init_game(blockWidth, &gameState);
    init_test();
    unsigned int ticks = 0;
    unsigned int prevTicks = 0;
    while (gameState != EXIT) {
        float deltaTime = ticks - prevTicks;
        prevTicks = ticks;
        process_input(&gameState);
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
