#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Texture.hpp"
#include "RenderWindow.hpp"

double getMoveAngle(const bool &f, const bool &b, const bool &l, const bool &r) {
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

struct {
    double g = 9.80665;
} Constants;

template <typename ArithType> std::pair<ArithType, double> makeVector(const ArithType &x, const ArithType &y, const bool &useRadians = true) {
    static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
    return {std::is_integral<ArithType>::value ? std::round(std::sqrt(x * x + y * y)) : std::sqrt(x * x + y * y), std::atan2(y, x) * (useRadians ? 1 : 180 / M_PI)};
}
template <typename ArithType> std::pair<ArithType, ArithType> breakVector(const ArithType &mag, const double &angle, const bool &useRadians = true) {
    static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
    if (std::is_integral<ArithType>::value) {return {std::round(mag * std::cos(angle * (useRadians ? 1 : 180 / M_PI))), std::round(mag * std::sin(angle * (useRadians ? 1 : 180 / M_PI)))};}
    return {mag * std::cos(angle * (useRadians ? 1 : M_PI / 180)), mag * std::sin(angle * (useRadians ? 1 : M_PI / 180))};
}

namespace kmtcs {
    template <typename ArithType> ArithType airTime(const ArithType &mag, const double &angle, const ArithType &dy, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        const double vy = mag * std::sin(angle * (useRadians ? 1 : M_PI / 180));
        const double root = std::sqrt(vy * vy - 2 * Constants.g * dy);
        return std::is_integral<ArithType>::value ? std::round(std::fmax((-vy + root) / -Constants.g, (-vy - root) / -Constants.g)) : std::fmax((-vy + root) / -Constants.g, (-vy - root) / -Constants.g);
    }

    template <typename ArithType> ArithType peakTime(const ArithType &mag, const double &angle, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        return std::is_integral<ArithType>::value ? std::round((mag * std::sin(angle * (useRadians ? 1 : M_PI / 180))) / Constants.g) : ((mag * std::sin(angle * (useRadians ? 1 : M_PI / 180))) / Constants.g);
    }

    template <typename ArithType> ArithType maxHeight(const ArithType &mag, const double &angle, const ArithType &y0, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        const double peak = peakTime<double>(mag, angle, useRadians);
        return std::is_integral<ArithType>::value ? std::round(y0 + mag * std::sin(angle * (useRadians ? 1 : M_PI / 180)) * peak - 0.5 * Constants.g * peak * peak) : (y0 + mag * std::sin(angle * (useRadians ? 1 : M_PI / 180)) * peak - 0.5 * Constants.g * peak * peak);
    }

    template <typename ArithType> double launchAngle(const ArithType &dx, const ArithType &dy, const ArithType &mag, const bool &minimizeHeight = true, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        const double root = std::sqrt(mag * mag * mag * mag - Constants.g * (Constants.g * dx * dx + 2 * dy * mag * mag));
        const double a1 = std::atan2(mag * mag + root, Constants.g * dx);
        const double a2 = std::atan2(mag * mag - root, Constants.g * dx);
        if (minimizeHeight) {
            if (maxHeight<ArithType>(mag, a1, 0, useRadians) < maxHeight<ArithType>(mag, a2, 0, useRadians)) {return a1 * (useRadians ? 1 : 180 / M_PI);}
            return a2 * (useRadians ? 1 : 180 / M_PI);
        }
        if (maxHeight<ArithType>(mag, a1, 0, useRadians) < maxHeight<ArithType>(mag, a2, 0, useRadians)) {return a2 * (useRadians ? 1 : 180 / M_PI);}
        return a1 * (useRadians ? 1 : 180 / M_PI);
    }

    template <typename ArithType> std::pair<ArithType, double> landingVector(const ArithType &mag, const double &angle, const ArithType &dy, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        return makeVector<ArithType>(std::is_integral<ArithType>::value ? std::round(mag * std::cos(angle * (useRadians ? 1 : M_PI / 180))) : (mag * std::cos(angle * (useRadians ? 1 : M_PI / 180))), mag * std::sin(angle * (useRadians ? 1 : M_PI / 180)) - Constants.g * airTime<double>(mag, angle, dy, useRadians), useRadians);
    }
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

    const double strength = 150;
    double throwAngle;
    std::pair<double, double> strengthComps;

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
                double moveAngle = getMoveAngle(keystate[keybinds.moveForwards], keystate[keybinds.moveBackwards], keystate[keybinds.strafeLeft], keystate[keybinds.strafeRight]);
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
        // Window.drawCircle(0, 0, pos.distEuclid(), keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT] ? DefaultColors[COLOR_LIME] : DefaultColors[COLOR_WHITE]);
        // Window.drawCircle(MousePos.x, MousePos.y, std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY())), keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN] ? DefaultColors[COLOR_LIME] : DefaultColors[COLOR_WHITE]);
        Window.renderTexture(texture, dst);
        Window.renderTexture(block, {0, 0, 64, 64});

        throwAngle = kmtcs::launchAngle<double>(MousePos.x - pos.getX(), MousePos.y - pos.getY(), strength, true);
        if (!std::isnan(throwAngle)) {
            strengthComps = breakVector<double>(strength, throwAngle);

            double dx = 0.05;
            double cx = 0;
            double cy = 0;
            double nx = 0;
            double ny = 0;
            for (double i = 0; i < (MousePos.x - pos.getX()) / strengthComps.first; i += dx) {
                nx += dx;
                ny = -0.5 * Constants.g * nx * nx + strengthComps.second * nx;
                Window.drawLine(dst.x + cx * strengthComps.first, dst.y + cy, dst.x + nx * strengthComps.first, dst.y + ny, DefaultColors[COLOR_RED]);
                cx = nx;
                cy = ny;
            }
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
