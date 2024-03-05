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

    SDL_SetRelativeMouseMode(SDL_TRUE);

    RenderWindow Window("le windo", 1280, 720);
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    // Overall time used for physics simulation
    double t = 0.0;
    // Amount of seconds to pass in between 'physics frames'
    // At 0.01, there should be 100 'physics frames' each second
    double dt = 0.01;

    // WHEN IMPLEMENTED: State previousState;
    // WHEN IMPLEMENTED: State currentState;

    // Used to ensure that the program doesn't run too fast
    int startTicks = 0, frameTicks = 0;

    // Junk that probably has a purpose when simulating things
    double currentTime = hireTime_Sec();
    double newTime = 0.0;
    double frameTime = 0.0;
    double accumulator = 0.0;

    //
    // NON-BOILER-PLATE
    //

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
    double px = 0, py = 0;

    bool running = true;
    while (running) {
        // Get the ticks before computation
        startTicks = SDL_GetTicks();

        // More jargon for simulations
        newTime = hireTime_Sec();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            if (keystate[SDL_SCANCODE_ESCAPE]) {running = false;}
            if (keystate[SDL_SCANCODE_W]) {
                ay++;
            } else if (keystate[SDL_SCANCODE_S]) {
                ay--;
            }
            if (keystate[SDL_SCANCODE_A]) {
                az++;
            } else if (keystate[SDL_SCANCODE_D]) {
                az--;
            }
            if (keystate[SDL_SCANCODE_Q]) {
                ax++;
            } else if (keystate[SDL_SCANCODE_E]) {
                ax--;
            }
            if (keystate[SDL_SCANCODE_1]) {
                focalLength--;
            } else if (keystate[SDL_SCANCODE_2]) {
                focalLength++;
            }
            if (keystate[SDL_SCANCODE_UP]) {
                py++;
            } else if (keystate[SDL_SCANCODE_DOWN]) {
                py--;
            }
            if (keystate[SDL_SCANCODE_LEFT]) {
                px--;
            } else if (keystate[SDL_SCANCODE_RIGHT]) {
                px++;
            }

            // WHEN IMPLEMENTED: previousState = currentState;
            // WHEN IMPLEMENTED: simulatePhysics(currentState, t, dt);
            
            // By calculating the angle that the line should be drawn at here, the angular speed should remain constant across different fps
            // With how it is now, it should make a full rotation every 3.6 seconds
            for (unsigned char i = 0; i < points.size(); i++) {
                pointsNow[i] = rotateCoord_3D(Vector_3D<int>(0, 1, 0), points[i], ay / 5);
                pointsNow[i] = rotateCoord_3D(Vector_3D<int>(0, 0, 1), pointsNow[i], az / 5);
                pointsNow[i] = rotateCoord_3D(Vector_3D<int>(1, 0, 0), pointsNow[i], ax / 5);
            }

            for (unsigned char i = 0; i < points.size(); i++) {
                projected[i] = projectCoord<int>(pointsNow[i] + Coord_3D<int>(0, px / 4, py / 4), focalLength);
            }

            // Even more simulation jargon
            t += dt;
            accumulator -= dt;
        }

        // WHEN IMPLEMENTED: const double alpha = accumulator / dt;
        // WHEN IMPLEMENTED: State renderState = currentState * alpha + previousState * (1.0 - alpha);
        // WHEN IMPLEMENTED: render(renderState);

        Window.clear();
        
        for (unsigned char i = 0; i < pairs.size(); i++) {
            Window.drawLine(projected[pairs[i].first].getY(), projected[pairs[i].first].getZ(), projected[pairs[i].second].getY(), projected[pairs[i].second].getZ());
        }
        for (unsigned char i = 0; i < projected.size(); i++) {
            Window.drawRectangle(projected[i].getY() - 2, projected[i].getZ() + 2, 5, 5, DefaultColors[5 + i]);
        }

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
