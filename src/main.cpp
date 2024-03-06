#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "RenderWindow.hpp"
#include "General.hpp"
#include "astr.hpp"

double hireTime_Sec() {return SDL_GetTicks() * 0.01f;}

template <typename ArithType> Coord_3D<ArithType> projectCoord(const Coord_3D<ArithType> &coord, double focalLength) {
    static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

    // Protect from a floating-point exception, even though a projection doesn't occur
    if (focalLength + coord.getX() == 0) {return coord;}

    Coord_3D<ArithType> output;

    output.setY((focalLength * coord.getY()) / (focalLength + coord.getX()));
    output.setZ((focalLength * coord.getZ()) / (focalLength + coord.getX()));

    return output;
}

template <typename ArithType> Coord_3D<ArithType> rotateCoord_3D(const Vector_3D<ArithType> &axis, const Coord_3D<ArithType> &coord, double theta, bool degrees = true) {
    static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

    // Convert the angle to radians if needed
    if (degrees) {theta *= M_PI / 180;}

    ArithType x = axis.getX();
    ArithType y = axis.getY();
    ArithType z = axis.getZ();

    const std::vector<std::vector<double>> R = {
        {std::cos(theta) + std::pow(x, 2) * (1 - std::cos(theta)), x * y * (1 - std::cos(theta)) - z * std::sin(theta), x * z * (1 - std::cos(theta)) + y * std::sin(theta)},
        {y * x * (1 - std::cos(theta)) + z * std::sin(theta), std::cos(theta) + std::pow(y, 2) * (1 - std::cos(theta)), y * z * (1 - std::cos(theta)) - x * std::sin(theta)},
        {z * x * (1 - std::cos(theta)) - y * std::sin(theta), z * y * (1 - std::cos(theta)) + x * std::sin(theta), std::cos(theta) + std::pow(z, 2) * (1 - std::cos(theta))}
    };

    std::vector<ArithType> original = coord.toVector();
    std::vector<double> rotated = {0, 0, 0};
    for (unsigned char i = 0; i < 3; i++) {
        for (unsigned char j = 0; j < 3; j++) {
            rotated[i] += original[j] * R[i][j];
        }
    }

    if (std::is_floating_point<ArithType>::value) {
        return Coord_3D<ArithType>(rotated[0], rotated[1], rotated[2]);
    }
    
    return Coord_3D<ArithType>(std::round(rotated[0]), std::round(rotated[1]), std::round(rotated[2]));
}

int main(int argc, char* args[]) {
    std::cout << "\n";

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2 successfully initialized\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}
    else {std::cout << "SDL2_image successfully initialized\n";}

    RenderWindow Window("le windo - cube", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = hireTime_Sec();
    double newTime = 0.0;
    double frameTime = 0.0;
    double accumulator = 0.0;

    //
    // NON-BOILER-PLATE
    //

    unsigned char mode = 0;
    const char* titles[2] = {"le windo - cube", "le windo - fella"};

    std::vector<std::pair<unsigned char, unsigned char>> pairs = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
    std::vector<Coord_3D<int>> points = {
        Coord_3D<int>(-100, -100,  100),
        Coord_3D<int>(-100,  100,  100),
        Coord_3D<int>( 100,  100,  100),
        Coord_3D<int>( 100, -100,  100),
        Coord_3D<int>(-100, -100, -100),
        Coord_3D<int>(-100,  100, -100),
        Coord_3D<int>( 100,  100, -100),
        Coord_3D<int>( 100, -100, -100)
    };

    std::vector<Coord_3D<int>> pointsNow, projected;
    for (unsigned char i = 0; i < points.size(); i++) {
        pointsNow.emplace_back(points[i]);
        projected.emplace_back();
    }

    double focalLength = 400;
    double ax = 0, ay = 0, az = 0;
    double angleMult = 0.2;
    double cubeMoveSpeed = 2;
    double px = 0, py = 0;

    double fellaX = Window.getWidth() / 2, fellaY = Window.getHeight() / 2;
    double fellaMoveSpeed = 0.25;
    int mousePosX = 0, mousePosY = 0;

    bool mouseMotion = false;
    bool mouseFirst = true;
    bool mouseCaptured = false;
    double mouseX = 0, mouseY = 0, sensitivity = 2;

    bool running = true;
    while (running) {
        startTicks = SDL_GetTicks();
        newTime = hireTime_Sec();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }

                // Placing things here will add a small delay between repeats, so is better for keybinds that do a toggle rather than increment
                if (event.type == SDL_KEYDOWN) {
                    // Toggle between having a captive mouse (usually for camera movement) or a standard mouse
                    if (keystate[SDL_SCANCODE_GRAVE]) {
                        mouseCaptured = !mouseCaptured;
                        if (mouseCaptured) {
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                            mouseFirst = true;
                            mouseX = mouseY = 0;
                        } else {
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                            Window.centerMouse();
                        }
                    }
                    // Change the mode
                    if (keystate[SDL_SCANCODE_Z]) {
                        mode = mode == 0 ? 0 : mode - 1;
                        Window.changeTitle(titles[mode]);
                    } else if (keystate[SDL_SCANCODE_X]) {
                        mode = mode == 1 ? 1 : mode + 1;
                        Window.changeTitle(titles[mode]);
                    }
                    // Toggle fullscreen
                    if (keystate[SDL_SCANCODE_F11]) {
                        Window.toggleFullscreen();
                    }
                }

                SDL_GetMouseState(&mousePosX, &mousePosY);

                switch (mode) {
                    case 0:
                        // Detect mouse movement and translate that into a variable that can be used to capture the speed of the mouse
                        if (event.type == SDL_MOUSEMOTION) {
                            mouseMotion = true;
                            if (!mouseFirst) {
                                mouseX = event.motion.xrel;
                                mouseY = event.motion.yrel;
                            } else {
                                mouseFirst = false;
                                mouseX = mouseY = 0;
                            }
                        }
                        break;
                }
            }
            // Keybind to leave window/program (outside of a quit event like mod+shift+q or alt+f4)
            if (keystate[SDL_SCANCODE_ESCAPE]) {running = false;}

            switch (mode) {
                case 0:
                    // Change the angles of the cube based on mouse movement/keyboard input
                    if (mouseMotion && mouseCaptured) {
                        mouseMotion = false;
                        az -= mouseX * sensitivity * angleMult;
                        ay -= mouseY * sensitivity * angleMult;
                    }
                    if (keystate[SDL_SCANCODE_W]) {
                        ay += angleMult;
                    } else if (keystate[SDL_SCANCODE_S]) {
                        ay -= angleMult;
                    }
                    if (keystate[SDL_SCANCODE_A]) {
                        az += angleMult;
                    } else if (keystate[SDL_SCANCODE_D]) {
                        az -= angleMult;
                    }
                    if (keystate[SDL_SCANCODE_Q]) {
                        ax += angleMult;
                    } else if (keystate[SDL_SCANCODE_E]) {
                        ax -= angleMult;
                    }
                    // Make sure to constrain the angles for long-term use and general conveinience
                    if (ax > 360) {ax -= 360;}
                    if (ax < 0) {ax += 360;}
                    if (ay > 360) {ay -= 360;}
                    if (ay < 0) {ay += 360;}
                    if (az > 360) {az -= 360;}
                    if (az < 0) {az += 360;}

                    // Get further away from or closer to the cube
                    if (keystate[SDL_SCANCODE_1]) {
                        focalLength--;
                    } else if (keystate[SDL_SCANCODE_2]) {
                        focalLength++;
                    }
                    // Move the cube in the four 2d-directions w/ arrow keys
                    if (keystate[SDL_SCANCODE_UP]) {
                        py += cubeMoveSpeed;
                    } else if (keystate[SDL_SCANCODE_DOWN]) {
                        py -= cubeMoveSpeed;
                    }
                    if (keystate[SDL_SCANCODE_LEFT]) {
                        px -= cubeMoveSpeed;
                    } else if (keystate[SDL_SCANCODE_RIGHT]) {
                        px += cubeMoveSpeed;
                    }

                    // Rotate the 3d cube based on angles and then project the new cube onto the 2d screen for actual rendering
                    for (unsigned char i = 0; i < points.size(); i++) {
                        pointsNow[i] = rotateCoord_3D(Vector_3D<int>(0, 1, 0), points[i], ay);
                        pointsNow[i] = rotateCoord_3D(Vector_3D<int>(0, 0, 1), pointsNow[i], az);
                        pointsNow[i] = rotateCoord_3D(Vector_3D<int>(1, 0, 0), pointsNow[i], ax);
                    }
                    for (unsigned char i = 0; i < points.size(); i++) {
                        projected[i] = projectCoord<int>(pointsNow[i] + Coord_3D<int>(0, px / 4, py / 4), focalLength);
                    }
                    break;
                case 1:
                    if (keystate[SDL_SCANCODE_W]) {
                        fellaY += fellaMoveSpeed;
                    } else if (keystate[SDL_SCANCODE_S]) {
                        fellaY -= fellaMoveSpeed;
                    }
                    if (keystate[SDL_SCANCODE_A]) {
                        fellaX -= fellaMoveSpeed;
                    } else if (keystate[SDL_SCANCODE_D]) {
                        fellaX += fellaMoveSpeed;
                    }
                    break;
            }

            t += dt;
            accumulator -= dt;
        }

        // Clear the previous frame
        Window.clear();

        switch (mode) {
            case 0:
                // Display the wireframe cube
                for (unsigned char i = 0; i < pairs.size(); i++) {
                    Window.drawLine(projected[pairs[i].first].getY(), projected[pairs[i].first].getZ(), projected[pairs[i].second].getY(), projected[pairs[i].second].getZ());
                }
                for (unsigned char i = 0; i < projected.size(); i++) {
                    Window.drawRectangle(projected[i].getY() - 2, projected[i].getZ() + 2, 5, 5, DefaultColors[5 + i]);
                }
                break;
            case 1:
                Window.drawLine(fellaX - Window.getWidth() / 2, fellaY - Window.getHeight() / 2, mousePosX - Window.getWidth() / 2, Window.getHeight() / 2 - mousePosY);
                break;
        }

        // Show the new frame
        Window.show();

        // Get the ticks after computation and then delay things based on refresh rate
        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    std::cout << "\n";
    SDL_Quit();
    return 0;    
}
