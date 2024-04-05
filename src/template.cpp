#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "DefaultColors.hpp"

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    // if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}
    // if (!TTF_Init()) {std::cout << "Error initializing SDL2_ttf\nERROR: " << SDL_GetError() << "\n";}

    RenderWindow Window("window", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    struct {
        SDL_Point Pos = {0, 0};
        SDL_Point Rel = {0, 0};
        double Sensitivity = 1;

        bool Motion = false;
        bool First = true;
        bool Captured = false;
    } MouseInfo;

    struct {
        int ToggleCapture = SDL_SCANCODE_GRAVE;
    } Keybinds;

    long double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    long double newTime = 0.0;
    long double frameTime = 0.0;
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
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_MOUSEMOTION:
                        MouseInfo.Motion = true;
                        SDL_GetMouseState(&MouseInfo.Pos.x, &MouseInfo.Pos.y);
                        MouseInfo.Pos.x = MouseInfo.Pos.x - Window.getW_2();
                        MouseInfo.Pos.y = Window.getH() - MouseInfo.Pos.y - Window.getH_2();

                        if (!MouseInfo.First) {
                            MouseInfo.Rel.x = event.motion.xrel;
                            MouseInfo.Rel.y = event.motion.yrel;
                        } else {
                            MouseInfo.First = false;
                            MouseInfo.Rel = {0, 0};
                        }
                        break;
                    case SDL_KEYDOWN:
                        if (keystate[Keybinds.ToggleCapture]) {

                        }
                        break;
                    case SDL_WINDOWEVENT:
                        Window.handleEvent(event.window);
                        break;
                }
                if (!running) {break;}
            }
            if (!running || keystate[SDL_SCANCODE_ESCAPE]) {
                running = false;
                break;
            }

            t += dt;
            accumulator -= dt;
        }
        if (!running) {break;}

        Window.clear();
        Window.show();

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    // TTF_Quit();
    // IMG_Quit();
    SDL_Quit();
    return 0; 
}
