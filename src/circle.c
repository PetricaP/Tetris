#include "circle.h"
#include "window.h"

// mid-point algorithm from wikipedia
void draw_circle(vec2i position, unsigned int radius, vec4u color) {
    SDL_Color clearColor = get_clear_color();
    SDL_SetRenderDrawColor(get_renderer(), color.r, color.g, color.b,
                           color.a);
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);
    while (x >= y)
    {
		SDL_RenderDrawLine(get_renderer(), position.x + x, position.y + y,
                                           position.x - x, position.y + y);
		SDL_RenderDrawLine(get_renderer(), position.x - y, position.y + x,
                                           position.x + y, position.y + x);
		SDL_RenderDrawLine(get_renderer(), position.x - x, position.y - y,
                                           position.x + x, position.y - y);
		SDL_RenderDrawLine(get_renderer(), position.x + y, position.y - x,
                                           position.x - y, position.y - x);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
    SDL_SetRenderDrawColor(get_renderer(), clearColor.r, clearColor.g,
                           clearColor.b, clearColor.a);
}
