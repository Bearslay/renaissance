#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "RenderWindow.hpp"
#include "SDLColor.hpp"
#include "astr.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height) : Window(NULL), Renderer(NULL), Width(width), Height(height) {
    Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (Window == NULL) {std::cout << "Window \"" << title << "\" failed to initialize\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL_Window \"" << title << "\" successfully created\n";}

    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if (Renderer == NULL) {std::cout << "Renderer for \"" << title << "\" failed to initialize\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL_Renderer for \"" << title << "\" successfully created\n";}
}

RenderWindow::~RenderWindow() {
    // std::cout << "SDL_Window \"" << SDL_GetWindowTitle(Window) << "\" successfully destroyed\n";
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
}

void RenderWindow::updateDims() {SDL_GetWindowSize(Window, &Width, &Height);}

int RenderWindow::getRefreshRate() {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(Window), 0, &mode);
    return mode.refresh_rate;
}

void RenderWindow::centerMouse() {SDL_WarpMouseInWindow(Window, Width / 2, Height / 2);}

void RenderWindow::changeTitle(const char* newTitle) {SDL_SetWindowTitle(Window, newTitle);}

bool RenderWindow::toggleFullscreen(bool trueFullscreen) {
    bool output = IsFullscreen;
    if (!IsFullscreen) {
        if (trueFullscreen) {
            SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);
        } else {
            SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);
        }
        IsFullscreen = true;
    } else {
        SDL_SetWindowFullscreen(Window, SDL_FALSE);
        IsFullscreen = false;
    }
    updateDims();
    return output;
}

int RenderWindow::getWidth() {return Width;}
int RenderWindow::getHeight() {return Height;}

void RenderWindow::clear(Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(Renderer);
}

void RenderWindow::show() {SDL_RenderPresent(Renderer);}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(Renderer, x1, Height - y1, x2, Height - y2);
}

// void RenderWindow::drawLineWeight(int x1, int y1, int x2, int y2, int weight, Color color) {
//     SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
//     SDL_RenderSetScale(Renderer, weight, weight);
//     SDL_RenderDrawLine(Renderer, x1 / weight, y1 / weight, x2 / weight, y2 / weight);
//     SDL_RenderSetScale(Renderer, 1, 1);
// }

void RenderWindow::drawPixel(int x, int y, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(Renderer, x, Height - y);
}

void RenderWindow::drawRectangle(int x, int y, int width, int height, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rectangle = {x, Height - y, width, height};
    SDL_RenderDrawRect(Renderer, &rectangle);
}

void RenderWindow::fillRectangle(int x, int y, int width, int height, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rectangle = {x, Height - y, width, height};
    SDL_RenderFillRect(Renderer, &rectangle);
}

void RenderWindow::drawCircle(int x, int y, int radius, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    int diameter = radius * 2;
    int ox = radius - 1;
    int oy = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - diameter;

    while (ox >= oy) {
        SDL_RenderDrawPoint(Renderer, x + ox, Height - y - oy);
        SDL_RenderDrawPoint(Renderer, x + ox, Height - y + oy);
        SDL_RenderDrawPoint(Renderer, x - ox, Height - y - oy);
        SDL_RenderDrawPoint(Renderer, x - ox, Height - y + oy);
        SDL_RenderDrawPoint(Renderer, x + oy, Height - y - ox);
        SDL_RenderDrawPoint(Renderer, x + oy, Height - y + ox);
        SDL_RenderDrawPoint(Renderer, x - oy, Height - y - ox);
        SDL_RenderDrawPoint(Renderer, x - oy, Height - y + ox);

        if (error <= 0) {
            oy++;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            ox--;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void RenderWindow::fillCircle(int x, int y, int radius, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    int ox = 0;
    int oy = radius;
    int error = radius - 1;

    while (oy >= ox) {
        SDL_RenderDrawLine(Renderer, x - oy, Height - y + ox, x + oy, Height - y + ox);
        SDL_RenderDrawLine(Renderer, x - ox, Height - y + oy, x + ox, Height - y + oy);
        SDL_RenderDrawLine(Renderer, x - ox, Height - y - oy, x + ox, Height - y - oy);
        SDL_RenderDrawLine(Renderer, x - oy, Height - y - ox, x + oy, Height - y - ox);

        if (error >= ox * 2) {
            error -= ox * 2 + 1;
            ox++;
        } else if (error < 2 * (radius - oy)) {
            error += oy * 2 - 1;
            oy--;;
        } else {
            error += 2 * (oy - ox - 1);
            oy--;
            ox++;
        }
    }
}
