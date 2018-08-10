#include "game.h"
#include "piece.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    const unsigned int screenWidth = 600;
    const unsigned int screenHeight = 810;
    const unsigned int blockWidth = 30;
    init_graphics();
    Window window = create_window("Hello", screenWidth, screenHeight);
    set_texture_source("./res/red_block.png", "res/blue_block.png",
                       "./res/green_block.png", "res/yellow_block.png");
    init_game(blockWidth);
    set_window_clear_color(140, 120, 120, 200);
    GameState gameState = PLAY;
    while (gameState != EXIT) {
        process_input(&gameState);
        if (gameState != PAUSE) {
            update_game();
        }
        draw_game();
        cap_fps(60);
        clear_window();
    }
    delete_window(&window);
    close_game();
    return 0;
}
