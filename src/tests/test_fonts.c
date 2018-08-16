#include "game/game.h"
#include <framework/font_manager.h>
#include <framework/window.h>

static Window m_Window;

int main() {
    init_fonts();
    init_graphics();
    m_Window = create_window("FontRenderTest", 810, 600);
    init_game(30);
    set_font("res/Hack-Bold.ttf");
    SDL_Color color = { 255, 255, 255, 255 };
    vec2i position = {100, 100};
    GameState gameState = PAUSE;
    set_window_clear_color(0, 0, 0, 255);
    while(gameState != EXIT) {
        process_input(&gameState);
        print_text("Hello Tetris", color, position, 20, 30);
        cap_fps(20);
        clear_window();
    }
}
