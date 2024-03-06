#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "RenderWindow.hpp"
#include "General.hpp"
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

void RenderWindow::updateDims() {
    SDL_GetWindowSize(Window, &Width, &Height);
}

int RenderWindow::getRefreshRate() {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(Window), 0, &mode);
    return mode.refresh_rate;
}

void RenderWindow::centerMouse() {
    SDL_WarpMouseInWindow(Window, Width / 2, Height / 2);
}

void RenderWindow::changeTitle(const char* newTitle) {
    SDL_SetWindowTitle(Window, newTitle);
}

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

void RenderWindow::show() {
    SDL_RenderPresent(Renderer);
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLine(Renderer, Width / 2 + x1, Height / 2 - y1, Width / 2 + x2, Height / 2 - y2);
}

void RenderWindow::drawPixel(int x, int y, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawPoint(Renderer, Width / 2 + x, Height / 2 - y);
}

void RenderWindow::drawRectangle(int x, int y, int width, int height, Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rectangle = {Width / 2 + x, Height / 2 - y, width, height};
    SDL_RenderFillRect(Renderer, &rectangle);
}
