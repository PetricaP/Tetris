#ifndef FRAMEWORK_FONT_MANAGER_H
#define FRAMEWORK_FONT_MANAGER_H

#include "vec2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void init_fonts();
void set_font(const char *filePath);
TTF_Font *get_font(const char *filePath);
void print_text(const char *message, SDL_Color color, vec2i position,
                int letterWidth, int letterHeight);
SDL_Texture *generate_text_texture(const char *message, SDL_Color color);
void draw_text_texture(SDL_Texture *texture, const char *message, vec2i position,
                       int letterWidth, int letterHeight);

#endif // FRAMEWORK_FONT_MANAGER_H
