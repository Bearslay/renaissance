#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Texture.hpp"
#include "RenderWindow.hpp"

template <typename ArithType> double getMoveAngle(const bool &f, const bool &b, const bool &l, const bool &r) {
    if (f && !b) {
        if (l && !r) {return 3 * M_PI_4;}
        if (!l && r) {return M_PI_4;}
        return M_PI_2;
    }
    if (!f && b) {
        if (l && !r) {return 5 * M_PI_4;}
        if (!l && r) {return 7 * M_PI_4;}
        return 3 * M_PI_2;
    }
    if (l && !r) {return M_PI;}
    if (!l && r) {return 0;}
    return -1;
}

template <typename ArithType> ArithType quadratic(const ArithType &a, const ArithType &b, const ArithType &c, const ArithType &x) {return a * x * x + b * x + c;}
template <typename ArithType> ArithType dquadratic(const ArithType &a, const ArithType &b, const ArithType &x) {return 2 * a * x + b;}
template <typename ArithType> ArithType euler(const ArithType &x0, const ArithType &y0, const ArithType &dx, const ArithType &x) {
    ArithType cx = x0;
    ArithType y = y0;
    while (cx < x) {
        y = y + dx * dquadratic<ArithType>(-4.91, 0, cx);
        cx += dx;
    }
    return y;
}

long double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}

    RenderWindow Window("Game Thing", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    long double t = 0.0;
    double dt = 0.01;
    int startTicks = 0, frameTicks = 0;
    long double currentTime = HireTime_Sec();
    long double newTime = 0.0;
    double frameTime = 0.0;
    double accumulator = 0.0;

    struct {
        int exit = SDL_SCANCODE_ESCAPE;
        int moveForwards = SDL_SCANCODE_W;
        int moveBackwards = SDL_SCANCODE_S;
        int strafeLeft = SDL_SCANCODE_A;
        int strafeRight = SDL_SCANCODE_D;
    } keybinds;
    SDL_Point MousePos;

    SDL_Point dstSize = {32, 32};
    Texture texture(Window.loadTexture("dev/thingy/gfx/smile.png"), {dstSize.x / 2, dstSize.y / 2}, {0, 0, 64, 64});
    Coord2D<double> pos(0, 0);
    SDL_Rect dst;
    const double movespeed = 0.25;

    Texture block(Window.loadTexture("dev/thingy/gfx/tile.png"), {0, 0, 64, 64});

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
                        SDL_GetMouseState(&MousePos.x, &MousePos.y);
                        MousePos.x = MousePos.x - Window.getW_2();
                        MousePos.y = Window.getH() - MousePos.y - Window.getH_2();
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

            const double distFromMouse = std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY()));
            // texture.setAngle(std::atan2(MousePos.y - pos.getY(), MousePos.x - pos.getX()));
            if (1) {
                double moveAngle = getMoveAngle<double>(keystate[keybinds.moveForwards], keystate[keybinds.moveBackwards], keystate[keybinds.strafeLeft], keystate[keybinds.strafeRight]);
                if (moveAngle >= 0) {
                    // moveAngle += texture.getAngle() - M_PI_2;
                    pos += Coord2D<double>(std::cos(moveAngle), std::sin(moveAngle)) * movespeed;
                }
            } else if (dstSize.x / 2 + 4 < distFromMouse) {pos += Coord2D<double>(std::cos(texture.getAngle()), std::sin(texture.getAngle())) * movespeed;}

            if (pos.distEuclid() > 0) {
                if (keystate[SDL_SCANCODE_RIGHT]) {
                    pos.rotate(-movespeed / pos.distEuclid());
                } else if (keystate[SDL_SCANCODE_LEFT]) {
                    pos.rotate(movespeed / pos.distEuclid());
                }
            }
            if (distFromMouse > 0) {
                if (keystate[SDL_SCANCODE_UP]) {
                    pos.rotate(Coord2D<double>(MousePos.x, MousePos.y), -movespeed / distFromMouse);
                } else if (keystate[SDL_SCANCODE_DOWN]) {
                    pos.rotate(Coord2D<double>(MousePos.x, MousePos.y), movespeed / distFromMouse);
                }
            }

            t += dt;
            accumulator -= dt;
        }
        if (!running) {break;}

        Window.clear();

        if (dstSize.x / 2 + 4 >= std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY()))) {texture.setMods(DefaultColors[COLOR_YELLOW]);}
        else {texture.setMods(DefaultColors[COLOR_RED]);}

        dst = {(int)pos.getX(), (int)pos.getY(), dstSize.x, dstSize.y};
        Window.drawCircle(0, 0, pos.distEuclid(), keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT] ? DefaultColors[COLOR_LIME] : DefaultColors[COLOR_WHITE]);
        Window.drawCircle(MousePos.x, MousePos.y, std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY())), keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN] ? DefaultColors[COLOR_LIME] : DefaultColors[COLOR_WHITE]);
        Window.renderTexture(texture, dst);
        Window.renderTexture(block, {0, 0, 64, 64});

        int x0 = (int)pos.getX();
        int y0 = (int)pos.getY();
        int x1 = x0 + 1;
        int y1 = y0;
        for (int i = 0; i < 100; i++) {
            y1 = quadratic<int>(-4.91, 0, y0, x1);
            x0++;
            x1++;
            Window.drawLine(x0, y0, x1, y1);
            y0 = y1;
        }
        
        Window.show();

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }
    IMG_Quit();
    SDL_Quit();
    return 0; 
}
