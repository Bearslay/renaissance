#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Coord2D.hpp"
#include "Vector2D.hpp"
#include "RenderWindow.hpp"
#include "Ray2D.hpp"
#include "LineSeg2D.hpp"
#include "DefaultColors.hpp"
#include "astr.hpp"

#define DIAGMULT 0.707106781

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}

template <typename ArithType> ArithType mapVal(ArithType input, ArithType inMin, ArithType inMax, ArithType outMin, ArithType outMax) {return outMin + (input - inMin) * (outMax - outMin) / (inMax - inMin);}

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

int main() {
    std::cout << "\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2 successfully initialized\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2_image successfully initialized\n";}

    RenderWindow View("raycast - view", 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY);
    RenderWindow Layout("raycast - layout", 480, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    double t = 0.0;
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

    std::vector<std::vector<Uint8>> grid = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    Ray2D<double> player(Coord2D<double>(-Layout.getW_2() + 45, -Layout.getH_2() + 45), Vector2D<double>(0, 500));
    double movespeed = 0.075;
    double lookspeed = 0.2;
    double zoomspeed = 0.1;
    bool useAngle = true;
    int unitSize = 30;

    int fov = 60;
    int rectWidth = View.getW() / fov;

    std::vector<LineSeg2D<double>> segments = {
        // Start with the boundary walls
        LineSeg2D<double>(-Layout.getW_2() + 30, Layout.getH_2() - 30, Layout.getW_2() - 30, Layout.getH_2() - 30),
        LineSeg2D<double>(-Layout.getW_2() + 30, Layout.getH_2() - 30, -Layout.getW_2() + 30, -Layout.getH_2() + 30),
        LineSeg2D<double>(Layout.getW_2() - 30, -Layout.getH_2() + 30, Layout.getW_2() - 30, Layout.getH_2() - 30),
        LineSeg2D<double>(Layout.getW_2() - 30, -Layout.getH_2() + 30, -Layout.getW_2() + 30, -Layout.getH_2() + 30)
    };
    // Create walls for the other cells, skipping walls if they neighbor another cell
    for (unsigned long i = 1; i < grid.size() - 1; i++) {
        for (unsigned long j = 1; j < grid[i].size() - 1; j++) {
            if (grid[i][j] != 0) {
                if (grid[i - 1][j] == 0) {
                    segments.emplace_back(LineSeg2D<double>(j * unitSize, (grid.size() - 1 - i) * unitSize + 30, j * unitSize + 30, (grid.size() - 1 - i) * unitSize + 30));
                }
                if (grid[i + 1][j] == 0) {
                    segments.emplace_back(LineSeg2D<double>(j * unitSize + 30, (grid.size() - 1 - i) * unitSize, j * unitSize, (grid.size() - 1 - i) * unitSize));
                }
                if (grid[i][j - 1] == 0) {
                    segments.emplace_back(LineSeg2D<double>(j * unitSize, (grid.size() - 1 - i) * unitSize, j * unitSize, (grid.size() - 1 - i) * unitSize + 30));
                }
                if (grid[i][j + 1] == 0) {
                    segments.emplace_back(LineSeg2D<double>(j * unitSize + 30, (grid.size() - 1 - i) * unitSize + 30, j * unitSize + 30, (grid.size() - 1 - i) * unitSize));
                }
            }
        }
    }

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
            if (keystate[keybinds.exit]) {running = false;}

            const double moveAngle = getMoveAngle<double>(keystate[keybinds.moveForwards], keystate[keybinds.moveBackwards], keystate[keybinds.strafeLeft], keystate[keybinds.strafeRight]) + player.getViewAngle(false) - M_PI_2;
            if (moveAngle > 1) {
                player.moveX(movespeed * std::cos(moveAngle));
                player.moveY(moveAngle * std::sin(moveAngle));
            }

            if (keystate[SDL_SCANCODE_LEFT]) {
                player.lookCCW(lookspeed);
            } else if (keystate[SDL_SCANCODE_RIGHT]) {
                player.lookCW(lookspeed);
            }
            if (keystate[SDL_SCANCODE_UP]) {
                player.zoomOut(zoomspeed);
            } else if (keystate[SDL_SCANCODE_DOWN]) {
                player.zoomIn(zoomspeed);
            }

            t += dt;
            accumulator -= dt;
        }

        Layout.clear();
        View.clear();
        // Draw the maze
        for (unsigned long i = 0; i < grid.size(); i++) {
            for (unsigned j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == 1) {
                    Layout.fillRectangle(j * unitSize, (grid.size() - i) * unitSize, unitSize, unitSize, DefaultColors[COLOR_WHITE]);
                }
                // Layout.drawRectangle(j * unitSize, (grid.size() - i) * unitSize, unitSize, unitSize, DefaultColors[COLOR_DARK_GRAY]);
            }
        }

        Coord2D<double> intersection;
        double originalAngle = player.lookCCW(fov / 2 - (fov % 2 == 0 ? 0.5 : 0));
        // Draw the view lines
        for (int i = 0; i < fov + (fov % 2 == 0 ? 1 : 0); i++) {
            player.lookCW(1);
            // If the current ray hits a wall
            intersection = player.getIntersection(segments);
            if (!std::isnan(intersection.getX())) {
                Layout.drawLine(player.getPosX(), player.getPosY(), intersection.getX(), intersection.getY(), DefaultColors[COLOR_LIME]);

                Vector2D<double> projection(std::fabs(player.getPosX() - intersection.getX()), std::fabs(player.getPosY() - intersection.getY()));
                int distance = projection.getMag();
                distance *= std::cos(projection.getAngle(false));

                unsigned char rectBrightness = (unsigned char)mapVal<int>(distance, 0, (int)player.getViewMag(), 255, 0);
                int rectHeight = mapVal<int>(distance, 0, (int)player.getViewMag(), View.getH(), 0);
                View.fillRectangle(i * rectWidth, rectHeight / 2, rectWidth, rectHeight, {rectBrightness, rectBrightness, rectBrightness, 255});
            }
            // If the current ray doesn't hit a wall
            else {
                Layout.drawLine(player.getPosX(), player.getPosY(), player.getPosX() + player.getViewX(), player.getPosY() + player.getViewY(), DefaultColors[COLOR_GREEN]);
            }
        }
        player.setViewAngle(originalAngle);

        // Draw the line segments
        // for (unsigned char i = 0; i < segments.size(); i++) {
        //     Layout.drawLine(segments[i].getPos1X(), segments[i].getPos1Y(), segments[i].getPos2X(), segments[i].getPos2Y());
        // }

        // Draw the player
        // Layout.drawLine(player.getPosX(), player.getPosY(), player.getPosX() + player.getViewX(), player.getPosY() + player.getViewY(), DefaultColors[COLOR_CYAN]);
        Layout.fillCircle(player.getPosX(), player.getPosY(), 8, DefaultColors[COLOR_MAROON]);
        Layout.drawCircle(player.getPosX(), player.getPosY(), 8, DefaultColors[COLOR_RED]);
        Layout.show();
        View.show();

        // Prevent rendering faster than needing to
        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / View.getRefreshRate()) {
            SDL_Delay(1000 / View.getRefreshRate() - frameTicks);
        }
    }

    std::cout << "\n";
    SDL_Quit();
    return 0; 
}
