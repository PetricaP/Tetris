#include "list.h"
#include "font_manager.h"
#include "hash_map.h"
#include "window.h"
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

static Node_T *m_Fonts[13] = { NULL };
static TTF_Font *m_CurrentFont = NULL;

void set_font(const char *filePath) {
    m_CurrentFont = get_font(filePath);
    if (m_CurrentFont == NULL) {
        m_CurrentFont = TTF_OpenFont(filePath, 500);
        if (m_CurrentFont == NULL) {
            fprintf(stderr, "Couldn't load font from %s\n", filePath);
            exit(EXIT_FAILURE);
        }
        TTF_SetFontStyle(m_CurrentFont, TTF_STYLE_BOLD);
        insert(m_Fonts, filePath, m_CurrentFont);
    }
}

TTF_Font *get_font(const char *filePath) {
    return get(m_Fonts, filePath);
}

void init_fonts() {
    TTF_Init();
}

SDL_Texture *generate_text_texture(const char *message, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(m_CurrentFont,
                                                    message, color);
    if(textSurface == NULL){
        printf("Couldn't load textSurface. Error: %s", SDL_GetError());
        exit(1);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(get_renderer(), textSurface);
    if(textTexture == NULL){
        printf("Couldn't create textTexture. Error: %s", SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
}

void print_text(const char *message, SDL_Color color, vec2i position,
                int letterWidth, int letterHeight) {
    SDL_Texture *textTexture = generate_text_texture(message, color);
    int numLetters = strlen(message);
    int width = letterWidth * numLetters;
    int offset_x = width / 2;
    int offset_y = letterHeight / 2;
    SDL_Rect textRect = { position.x - offset_x, position.y - offset_y, width, letterHeight };
    SDL_RenderCopy(get_renderer(), textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void draw_text_texture(SDL_Texture *texture, const char *message, vec2i position,
                       int letterWidth, int letterHeight) {
    int numLetters = strlen(message);
    int width = letterWidth * numLetters;
    int offset_x = width / 2;
    int offset_y = letterHeight / 2;
    SDL_Rect textRect = { position.x - offset_x, position.y - offset_y, width, letterHeight };
    SDL_RenderCopy(get_renderer(), texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
    SDL_RenderCopy(get_renderer(), texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}
