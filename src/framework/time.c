#include "time.h"
#include <SDL2/SDL.h>


void cap_fps(unsigned int max_fps) {
    static unsigned int prevTicks = 0;
    unsigned int diff = SDL_GetTicks() - prevTicks;
    unsigned int wait = (1.0f / max_fps * 1000);
    if (diff < wait) {
        SDL_Delay(wait - diff);
    }
    prevTicks = SDL_GetTicks();
}

unsigned int get_ticks(void) { return SDL_GetTicks(); }
