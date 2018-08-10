#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *m_SDLWindow;
} Window;

Window create_window(const char *name, unsigned int width, unsigned int height);
SDL_Renderer *get_renderer(void);
void delete_window(Window *window);
void clear_window(void);
void set_window_clear_color(unsigned char r,  unsigned char g, unsigned char b,
        unsigned char a);
SDL_Color get_clear_color(void);

#endif
