#include "font_manager.h"
#include "hash_map.h"
#include "list.h"
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

static Node_T *m_Fonts[13];

void set_font(const char *filePath) {
    TTF_Font *font = TTF_OpenFont(filePath, 500);
    if (font == NULL) {
        fprintf(stderr, "Couldn't load font from %s\n", filePath);
        exit(EXIT_FAILURE);
    }
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    insert(m_Fonts, filePath, font);
}

TTF_Font *get_font(const char *filePath) {
    return get(m_Fonts, filePath);
}
