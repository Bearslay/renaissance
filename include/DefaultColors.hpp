#ifndef DEFAULTCOLORS
#define DEFAULTCOLORS

#include <SDL2/SDL.h>

const SDL_Color DefaultColors[16] = {
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

typedef enum {
    COLOR_BLACK = 0,
    COLOR_WHITE = 1,
    COLOR_LIGHT_GRAY = 2,
    COLOR_DARK_GRAY = 3,
    COLOR_BROWN = 4,
    COLOR_RED = 5,
    COLOR_MAROON = 6,
    COLOR_ORANGE = 7,
    COLOR_YELLOW = 8,
    COLOR_LIME = 9,
    COLOR_GREEN = 10,
    COLOR_CYAN = 11,
    COLOR_TEAL = 12,
    COLOR_BLUE = 13,
    COLOR_MAGENTA = 14,
    COLOR_PURPLE = 15
} DefaultColorsNames;

#endif /* DEFAULTCOLORS */
