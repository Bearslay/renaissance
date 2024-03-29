#ifndef TEXTURE
#define TEXTURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Coord2D.hpp"

class Texture {
    private:
        SDL_Texture* Image = NULL;
        Coord2D<float> Pos = Coord2D<float>(0, 0);
        SDL_Rect Frame = {0, 0, 0, 0};

    public:
        Texture(SDL_Texture* image, Coord2D<float> position, SDL_Rect frame) : Image(image), Pos(position), Frame(frame) {}

        SDL_Texture* getTexture() {return Image;}
        Coord2D<float> getPos() {return Pos;}
        SDL_Rect getFrame() {return Frame;}
};

#endif /* TEXTURE */
