#ifndef RENDERWINDOW
#define RENDERWINDOW

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "DefaultColors.hpp"
#include "Texture.hpp"

class RenderWindow {
    private:
        SDL_Window* Window;
        SDL_Renderer* Renderer;
        int Width = 0, Height = 0;
        bool IsFullscreen = false;

        void updateDims() {SDL_GetWindowSize(Window, &Width, &Height);}

    public:
        RenderWindow(const char* title, int width, int height) : Window(NULL), Renderer(NULL), Width(width), Height(height) {
            Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
            if (Window == NULL) {std::cout << "Window \"" << title << "\" failed to initialize\nERROR: " << SDL_GetError() << "\n";}
            else {std::cout << "SDL_Window \"" << title << "\" successfully created\n";}

            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
            if (Renderer == NULL) {std::cout << "Renderer for \"" << title << "\" failed to initialize\nERROR: " << SDL_GetError() << "\n";}
            else {std::cout << "SDL_Renderer for \"" << title << "\" successfully created\n";}
        }
        ~RenderWindow() {
            // std::cout << "SDL_Window \"" << SDL_GetWindowTitle(Window) << "\" successfully destroyed\n";
            SDL_DestroyRenderer(Renderer);
            SDL_DestroyWindow(Window);
        }

        int getRefreshRate() {
            SDL_DisplayMode mode;
            SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(Window), 0, &mode);
            return mode.refresh_rate;
        }
        int getWidth() {return Width;}
        int getHeight() {return Height;}

        void clear(SDL_Color color = DefaultColors[COLOR_BLACK]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
            SDL_RenderClear(Renderer);
        }
        void show() {SDL_RenderPresent(Renderer);}

        void centerMouse() {SDL_WarpMouseInWindow(Window, Width / 2, Height / 2);}
        void changeTitle(const char* newTitle) {SDL_SetWindowTitle(Window, newTitle);}
        bool toggleFullscreen(bool trueFullscreen = false) {
            bool output = IsFullscreen;
            if (!IsFullscreen) {
                if (trueFullscreen) {SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);}
                else {SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);}
                IsFullscreen = true;
            } else {
                SDL_SetWindowFullscreen(Window, SDL_FALSE);
                IsFullscreen = false;
            }
            updateDims();
            return output;
        }        

        void drawPixel(int x, int y, SDL_Color color = DefaultColors[COLOR_WHITE]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(Renderer, x, Height - y);
        }
        void drawLine(int x1, int y1, int x2, int y2, SDL_Color color = DefaultColors[COLOR_WHITE]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawLine(Renderer, x1, Height - y1, x2, Height - y2);
        }
        void drawRectangle(int x, int y, int width, int height, SDL_Color color = DefaultColors[COLOR_WHITE]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
            SDL_Rect rectangle = {x, Height - y, width, height};
            SDL_RenderDrawRect(Renderer, &rectangle);
        }
        void fillRectangle(int x, int y, int width, int height, SDL_Color color = DefaultColors[COLOR_WHITE]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
            SDL_Rect rectangle = {x, Height - y, width, height};
            SDL_RenderFillRect(Renderer, &rectangle);
        }
        void drawCircle(int x, int y, int radius, SDL_Color color = DefaultColors[COLOR_WHITE]) {
            SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

            const int diameter = radius * 2;
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
        void fillCircle(int x, int y, int radius, SDL_Color color = DefaultColors[COLOR_WHITE]) {
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

        SDL_Texture* loadTexture(const char* p_filePath) {
            SDL_Texture* texture = NULL;
            texture = IMG_LoadTexture(Renderer, p_filePath);

            if (texture == NULL) {std::cout << "Failed to load texture\nERROR: " << SDL_GetError() << "\n";}
            return texture;
        }

        void renderTexture(SDL_Texture* texture, const SDL_Rect &src, const SDL_Rect &dst, const double &angle, const SDL_Point &center, const SDL_RendererFlip &flip) {
            SDL_RenderCopyEx(Renderer, texture, &src, &dst, angle, &center, flip);
        }
        void renderTexture(const Texture &texture, const SDL_Rect &dst) {
            const SDL_Rect source = texture.getFrame();
            const SDL_Rect destination = {dst.x, Height - dst.y, dst.w, dst.h};
            const SDL_Point center = texture.getCenter();
            SDL_RenderCopyEx(Renderer, texture.getTexture(), &source, &destination, texture.getAngle(), &center, texture.getFlip());
        }
        void renderTexture(const Texture &texture, const SDL_Point &pos) {
            const SDL_Rect src = texture.getFrame();
            const SDL_Rect dst = {pos.x - texture.getFrame().w / 2, Height - pos.y - texture.getFrame().h / 2, texture.getFrame().w, texture.getFrame().h};
            const SDL_Point center = texture.getCenter();
            SDL_RenderCopyEx(Renderer, texture.getTexture(), &src, &dst, texture.getAngle(), &center, texture.getFlip());
        }
};

#endif /* RENDERWINDOW */
