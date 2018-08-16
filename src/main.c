#include "game.h"
#include "circle.h"
#include "particle.h"
#include "piece.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    const unsigned int screenWidth = 660;
    const unsigned int screenHeight = 810;
    const unsigned int blockWidth = 30;
    init_graphics();
    Window window = create_window("Hello", screenWidth, screenHeight);
    set_texture_source("./res/red_block.png", "res/blue_block.png",
                       "./res/green_block.png", "res/yellow_block.png");
    init_game(blockWidth);
    set_window_clear_color(20, 20, 40, 200);
    set_grid_clear_color(40, 20, 20, 200);
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
