#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "MathCoord.hpp"
#include "MathVector.hpp"
#include "RenderWindow.hpp"
#include "SDLColor.hpp"
#include "astr.hpp"

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}

int main() {
    std::cout << "\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2 successfully initialized\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2_image successfully initialized\n";}

    RenderWindow Window("topdown", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    double newTime = 0.0;
    double frameTime = 0.0;
    double accumulator = 0.0;

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

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    std::cout << "\n";
    SDL_Quit();
    return 0; 
}
