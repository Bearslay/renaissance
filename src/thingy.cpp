#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Coord2D.hpp"
#include "RenderWindow.hpp"

class TextureP {
    private:
        SDL_Texture* Texture;
        SDL_Point Center;
        SDL_Rect Frame;
        Uint8 RedMod = 255;
        Uint8 GreenMod = 255;
        Uint8 BlueMod = 255;
        Uint8 Opacity = 255;

    public:
        TextureP() : Texture(NULL), Center({}), Frame({}) {}
        TextureP(SDL_Texture* texture, SDL_Point center, SDL_Rect frame) : Texture(texture), Center(center), Frame(frame) {}
        TextureP(SDL_Texture* texture, SDL_Rect frame) : Texture(texture), Center({frame.w / 2, frame.h / 2}), Frame(frame) {}
        TextureP(SDL_Texture* texture, SDL_Point dims) : Texture(texture), Center({dims.x / 2, dims.y / 2}), Frame({0, 0, dims.x, dims.y}) {}


};

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main() {
    std::cout << "\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2 successfully initialized\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2_image successfully initialized\n";}

    RenderWindow Window("Game Thing", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    double t = 0.0;
    double dt = 0.01;
    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    double newTime = 0.0;
    double frameTime = 0.0;
    double accumulator = 0.0;

    SDL_Texture * texture = Window.loadTexture("dev/thingy/gfx/arrow.png");
    // SDL_SetTextureColorMod(texture, 255, 0, 255);
    // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // SDL_SetTextureAlphaMod(texture, 63);

    bool running = true;
    while (running) {
        startTicks = SDL_GetTicks();
        newTime = HireTime_Sec();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {running = false;}
            }
            if (keystate[SDL_SCANCODE_ESCAPE]) {running = false;}

            t += dt;
            accumulator -= dt;
        }

        Window.clear();
        Window.renderTexture(texture, {0, 0, 64, 64}, {64, 64, 64, 64}, -90, {32, 32}, SDL_FLIP_NONE);
        Window.show();

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    std::cout << "\n";
    SDL_Quit();
    return 0; 
}