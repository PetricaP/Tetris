#ifndef FRAMEWORK_FONT_MANAGER_H
#define FRAMEWORK_FONT_MANAGER_H

#include <SDL2/SDL_ttf.h>

void set_font(const char *filePath);
TTF_Font *get_font(const char *filePath);

#endif // FRAMEWORK_FONT_MANAGER_H
