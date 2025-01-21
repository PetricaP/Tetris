#include "framework/framework_time.h"
#include "framework/window.h"
#include "game/game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    const unsigned int screenWidth = 660;
    const unsigned int screenHeight = 810;
    const unsigned int blockWidth = 30;
    init_graphics();
    Window window = create_window("Tetris", screenWidth, screenHeight);
    GameState gameState = PLAY;
    init_game(blockWidth, &gameState);
    create_initial_pieces();
    set_window_clear_color(20, 20, 40, 200);
    set_grid_clear_color(40, 20, 20, 200);
    unsigned int ticks = 0;
    unsigned int prevTicks = 0;
    while (gameState != EXIT) {
        float deltaTime = ticks - prevTicks;
        prevTicks = ticks;
        process_input();
        if (gameState == PLAY) {
            update_game(deltaTime);
        }
        draw_game();
        cap_fps(60);
        clear_window();
        ticks = get_ticks(); 
    }
    delete_window(&window);
    close_game();
    return 0;
}
