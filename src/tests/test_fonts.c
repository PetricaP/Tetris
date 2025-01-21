#include "framework/framework_time.h"
#include "game/game.h"
#include <framework/font_manager.h>
#include <framework/window.h>


int main(void) {
    init_fonts();
    init_graphics();
    Window m_Window = create_window("FontRenderTest", 810, 600);
    set_font("res/Hack-Bold.ttf");
    set_window_clear_color(50, 50, 50, 255);
    SDL_Color color = { 255, 255, 255, 255 };
    vec2i position = {400, 200};
    GameState gameState = PAUSE;
    init_game(30, &gameState);
    while(gameState != EXIT) {
        process_input();
        print_text("Hello Tetris", color, position, 20, 30);
        cap_fps(20);
        clear_window();
    }
    delete_window(&m_Window);
    close_game();
}
