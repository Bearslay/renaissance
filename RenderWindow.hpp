#ifndef RENDERWINDOW
#define RENDERWINDOW

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "General.hpp"
#include "astr.hpp"

class RenderWindow {
    private:
        SDL_Window* Window;
        SDL_Renderer* Renderer;
        int Width = 0, Height = 0;

    public:
        RenderWindow(const char* title, int width, int height);
        ~RenderWindow();

        int getRefreshRate();

        void clear(Color color = {"black", 0, 0, 0, 0});
        void show();

        void drawLine(int x1, int y1, int x2, int y2, Color color = {"white", 1, 255, 255, 255, SDL_ALPHA_OPAQUE});
};

#endif /* RENDERWINDOW */
