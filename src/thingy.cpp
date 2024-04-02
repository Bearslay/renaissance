#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Texture.hpp"
#include "RenderWindow.hpp"

template <typename ArithType> Coord2D<ArithType> basicMove_TD(const Coord2D<ArithType> &pos, const ArithType &speed, const bool &forwards, const bool &backwards, const bool &left, const bool &right) {
    const double diagMult = 0.707106781;
    Coord2D<ArithType> output = pos;

    if (forwards && !backwards) {
        if (!left && !right) {
            output.moveY(speed);
        } else {
            if (left && !right) {
                output.moveX(-speed * diagMult);
                output.moveY(speed * diagMult);
            } else if (!left && right) {
                output.moveX(speed * diagMult);
                output.moveY(speed * diagMult);
            }
        }
    } else if (!forwards && backwards) {
        if (!left && !right) {
            output.moveY(-speed);
        } else {
            if (left && !right) {
                output.moveX(-speed * diagMult);
                output.moveY(-speed * diagMult);
            } else if (!left && right) {
                output.moveX(speed * diagMult);
                output.moveY(-speed * diagMult);
            }
        }
    } else if (!forwards && !backwards) {
        if (left && !right) {
            output.moveX(-speed);
        } else if (!left && right) {
            output.moveX(speed);
        }
    }

    return output;
}

template <typename ArithType> Coord2D<ArithType> basicMove_TD(const Coord2D<ArithType> &pos, const double &angle, const ArithType &speed, const bool &forwards, const bool &backwards, const bool &left, const bool &right) {
    const double diagMult = 0.707106781;
    Coord2D<ArithType> output = pos;
    const double sine = std::sin(angle);
    const double cosine = std::cos(angle);

    if (forwards && !backwards) {
        if (!left && !right) {
            output.moveX(speed * cosine);
            output.moveY(speed * sine);
        } else {
            if (left && !right) {
                output.moveX(speed * cosine * diagMult);
                output.moveY(speed * sine * diagMult);
                output.moveX(-speed * sine * diagMult);
                output.moveY(speed * cosine * diagMult);
            } else if (!left && right) {
                output.moveX(speed * cosine * diagMult);
                output.moveY(speed * sine * diagMult);
                output.moveX(speed * sine * diagMult);
                output.moveY(-speed * cosine * diagMult);
            }
        }
    } else if (!forwards && backwards) {
        if (!left && !right) {
            output.moveX(-speed * cosine);
            output.moveY(-speed * sine);
        } else {
            if (left && !right) {
                output.moveX(-speed * cosine * diagMult);
                output.moveY(-speed * sine * diagMult);
                output.moveX(-speed * sine * diagMult);
                output.moveY(speed * cosine * diagMult);
            } else if (!left && right) {
                output.moveX(-speed * cosine * diagMult);
                output.moveY(-speed * sine * diagMult);
                output.moveX(speed * sine * diagMult);
                output.moveY(-speed * cosine * diagMult);
            }
        }
    } else if (!forwards && !backwards) {
        if (left && !right) {
            output.moveX(-speed * sine);
            output.moveY(speed * cosine);
        } else if (!left && right) {
            output.moveX(speed * sine);
            output.moveY(-speed * cosine);
        }
    }

    return output;
}

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

    long double t = 0.0;
    double dt = 0.01;
    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    double newTime = 0.0;
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
    Texture texture(Window.loadTexture("dev/thingy/gfx/arrow.png"), {dstSize.x / 2, dstSize.y / 2}, {0, 0, 64, 64});
    Coord2D<double> pos = {(double)(Window.getWidth() / 2), (double)(Window.getHeight() / 2)};
    SDL_Rect dst;
    const double movespeed = 0.25;

    bool running = true;
    while (running) {
        startTicks = SDL_GetTicks();
        newTime = HireTime_Sec();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    break;
                }
                SDL_GetMouseState(&MousePos.x, &MousePos.y);
                MousePos.y = Window.getHeight() - MousePos.y;
            }
            if (!running || keystate[SDL_SCANCODE_ESCAPE]) {
                running = false;
                break;
            }

            texture.setAngle(std::atan2(MousePos.y - pos.getY(), MousePos.x - pos.getX()));
            pos = basicMove_TD<double>(pos, movespeed, keystate[keybinds.moveForwards], keystate[keybinds.moveBackwards], keystate[keybinds.strafeLeft], keystate[keybinds.strafeRight]);
            // if (dstSize.x / 2 + 4 < std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY()))) {pos = basicMove_TD<double>(pos, texture.getAngle(), movespeed, true, false, false, false);}

            t += dt;
            accumulator -= dt;
        }
        if (!running) {break;}

        Window.clear();

        if (dstSize.x / 2 + 4 >= std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY()))) {texture.setMods(DefaultColors[COLOR_YELLOW]);}
        else {texture.setMods(DefaultColors[COLOR_RED]);}

        dst = {(int)pos.getX(), (int)pos.getY(), dstSize.x, dstSize.y};
        Window.renderTexture(texture, dst);
        Window.show();

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    std::cout << "\n";
    IMG_Quit();
    SDL_Quit();
    return 0; 
}
