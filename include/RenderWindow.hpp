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
        bool IsFullscreen = false;

        void updateDims();

    public:
        RenderWindow(const char* title, int width, int height);
        ~RenderWindow();

        int getRefreshRate();
        void centerMouse();
        void changeTitle(const char* newTitle);
        bool toggleFullscreen(bool trueFullscreen = false);

        int getWidth();
        int getHeight();

        void clear(Color color = DefaultColors[COLOR_BLACK]);
        void show();

        void drawLine(int x1, int y1, int x2, int y2, Color color = DefaultColors[COLOR_WHITE]);
        void drawPixel(int x, int y, Color color = DefaultColors[COLOR_WHITE]);
        void drawRectangle(int x, int y, int width, int height, Color color = DefaultColors[COLOR_WHITE]);
        void drawCircle(int x, int y, int radius, Color color = DefaultColors[COLOR_WHITE]);
        void fillCircle(int x, int y, int radius, Color color = DefaultColors[COLOR_WHITE]);
};

#endif /* RENDERWINDOW */
