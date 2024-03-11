#ifndef SDLCOLOR
#define SDLCOLOR

#include <SDL2/SDL.h>
#include <vector>

#define COLOR_BLACK 0
#define COLOR_WHITE 1
#define COLOR_LIGHT_GRAY 2
#define COLOR_DARK_GRAY 3
#define COLOR_BROWN 4
#define COLOR_RED 5
#define COLOR_MAROON 6
#define COLOR_ORANGE 7
#define COLOR_YELLOW 8
#define COLOR_LIME 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_TEAL 12
#define COLOR_BLUE 13
#define COLOR_MAGENTA 14
#define COLOR_PURPLE 15

struct Color {Uint8 r = 0, g = 0, b = 0, a = SDL_ALPHA_OPAQUE;};
const std::vector<Color> DefaultColors = {
    {0, 0, 0, SDL_ALPHA_OPAQUE},
    {255, 255, 255, SDL_ALPHA_OPAQUE},
    {170, 170, 170, SDL_ALPHA_OPAQUE},
    {85, 85, 85, SDL_ALPHA_OPAQUE},
    {117, 60, 19, SDL_ALPHA_OPAQUE},
    {255, 0, 0, SDL_ALPHA_OPAQUE},
    {115, 0, 0, SDL_ALPHA_OPAQUE},
    {255, 115, 0, SDL_ALPHA_OPAQUE},
    {255, 255, 0, SDL_ALPHA_OPAQUE},
    {0, 255, 0, SDL_ALPHA_OPAQUE},
    {0, 115, 0, SDL_ALPHA_OPAQUE},
    {0, 255, 255, SDL_ALPHA_OPAQUE},
    {115, 115, SDL_ALPHA_OPAQUE},
    {0, 255, SDL_ALPHA_OPAQUE},
    {255, 0, 255, SDL_ALPHA_OPAQUE},
    {115, 0, 115, SDL_ALPHA_OPAQUE}
};

#endif /* SDLCOLOR */
