#include <framework/window.h>
#include <stdio.h>
#include <stdlib.h>

static SDL_Renderer *m_Renderer;
static unsigned int m_ScreenWidth;
static unsigned int m_ScreenHeight;
static SDL_Color m_ClearColor;

Window create_window(const char *name, unsigned int width, unsigned int height) {
    Window window;
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    window.m_SDLWindow = SDL_CreateWindow(name, 900, SDL_WINDOWPOS_CENTERED,
            width, height, 0);
    if (window.m_SDLWindow == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        exit(1);
    }
    m_Renderer = SDL_CreateRenderer(window.m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_Renderer == NULL) {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    return window;
}

SDL_Renderer *get_renderer(void) {
    return m_Renderer;
}

void clear_window(void) {
    SDL_RenderPresent(m_Renderer);
    SDL_RenderClear(m_Renderer);
}

void delete_window(Window *window) {
    SDL_DestroyWindow(window->m_SDLWindow);
    window->m_SDLWindow = NULL;
    SDL_DestroyRenderer(m_Renderer);
    m_Renderer = NULL;
}

SDL_Color get_clear_color(void) {
    return m_ClearColor;
}

void set_window_clear_color(unsigned char r, 
        unsigned char g, unsigned char b, unsigned char a) {
    m_ClearColor.r = r;
    m_ClearColor.g = g;
    m_ClearColor.b = b;
    m_ClearColor.a = a;
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
    SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
}

unsigned int get_screen_width(void) {
    return m_ScreenWidth;
}

unsigned int get_screen_height(void) {
    return m_ScreenHeight;
}
