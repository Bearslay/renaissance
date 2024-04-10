#include <SDL2/SDL.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "DefaultColors.hpp"

#include <vector>

#include "Coord2D.hpp"
#include "Coord3D.hpp"
#include "Vector3D.hpp"

#define EULER_ROLL 0
#define EULER_PITCH 1
#define EULER_YAW 2

template <typename ArithType> class WireFrame {
    private:
        Coord3D<ArithType> Origin = Coord3D<ArithType>(0, 0, 0);
        std::vector<Coord3D<ArithType>> BasePoints;
        std::vector<Coord3D<ArithType>> AlteredPoints;
        std::vector<Coord2D<ArithType>> ProjectedPoints;
        std::vector<std::pair<unsigned long, unsigned long>> Pairs;

        Vector3D<ArithType> Direction = Vector3D<ArithType>(1, 0, 0);

        void eulerRotate(const unsigned char &name, const double &angle) {
            const ArithType x, y, z;
            switch (name) {
                default:
                case EULER_ROLL:
                    x = Direction.getX();
                    y = Direction.getY();
                    z = Direction.getZ();
                    break;
                case EULER_PITCH:
                    x = Direction.getY();
                    y = Direction.getX();
                    z = Direction.getZ();
                case EULER_YAW:
                    x = Direction.getZ();
                    y = Direction.getY();
                    z = Direction.getX();
            }

            const double s = std::sin(angle);
            const double c = std::cos(angle);
            const std::vector<std::vector<double>> R = {
                {c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s},
                {y * x * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s},
                {z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c)}
            };

            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                const std::vector<ArithType> original = AlteredPoints[i].toVector();
                std::vector<double> rotated = {0, 0, 0};
                for (unsigned char j = 0; j < 3; j++) {
                    for (unsigned char k = 0; k < 3; k++) {
                        rotated[j] += original[k] * R[j][k];
                    }
                }

                if (std::is_floating_point<ArithType>::value) {AlteredPoints = Coord3D<ArithType>(rotated[0], rotated[1], rotated[2]);}
                else {AlteredPoints = Coord3D<ArithType>(std::round(rotated[0]), std::round(rotated[1]), std::round(rotated[2]));}
            }
        }

    public:
        WireFrame(const Coord3D<ArithType> &origin, const std::vector<Coord3D<ArithType>> &points, const std::vector<std::pair<unsigned long, unsigned long>> &pairs) : Origin(origin) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        
            for (unsigned long i = 0; i < points.size(); i++) {
                BasePoints.emplace_back(points[i]);
                AlteredPoints.emplace_back(points[i]);
                ProjectedPoints.emplace_back();
            }
            for (unsigned long i = 0; i < pairs.size(); i++) {
                Pairs.emplace_back(pairs[i]);
                if (Pairs[i].first  < 0 || Pairs[i].first  >= BasePoints.size()) {Pairs[i].first  = 0;}
                if (Pairs[i].second < 0 || Pairs[i].second >= BasePoints.size()) {Pairs[i].second = 0;}
            }
        }
        WireFrame(const Coord3D<ArithType> &origin, const std::vector<Coord3D<ArithType>> &points) {WireFrame(origin, points, {});}
        WireFrame(const Coord3D<ArithType> &origin) {WireFrame(origin, {}, {});}
        WireFrame(const WireFrame &wireFrame) {
            Origin;
        }
        WireFrame() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}

        Coord3D<ArithType> getPos() const {return Origin;} 
        ArithType            getX() const {return Origin.getX();}
        ArithType            getY() const {return Origin.getY();}
        ArithType            getZ() const {return Origin.getZ();}

        Coord3D<ArithType> setPos(const Coord3D<ArithType> &origin) {
            Coord3D<ArithType> output = Origin;
            Origin = origin;
            return output;
        }
        Coord3D<ArithType> adjPos(const Coord3D<ArithType> &amount) {
            Coord3D<ArithType> output = Origin;
            Origin += amount;
            return output;
        }
        ArithType setX(const ArithType &x) {
            ArithType output = Origin.getX();
            Origin.setX(x);
            return output;
        }
        ArithType adjX(const ArithType &amount) {
            ArithType output = Origin.getX();
            Origin.adjX(amount);
            return output;
        }
        ArithType setY(const ArithType &y) {
            ArithType output = Origin.getY();
            Origin.setY(y);
            return output;
        }
        ArithType adjY(const ArithType &amount) {
            ArithType output = Origin.getY();
            Origin.adjY(amount);
            return output;
        }
        ArithType setZ(const ArithType &z) {
            ArithType output = Origin.getZ();
            Origin.setZ(z);
            return output;
        }
        ArithType adjZ(const ArithType &amount) {
            ArithType output = Origin.getZ();
            Origin.adjZ(amount);
            return output;
        }

        void shiftPoints(const Coord3D<ArithType> &amount) {
            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                BasePoints[i] += amount;
            }
        }
        void shiftPointsX(const ArithType &amount) {
            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                BasePoints[i].adjX(amount);
            }
        }
        void shiftPointsY(const ArithType &amount) {
            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                BasePoints[i].adjY(amount);
            }
        }
        void shiftPointsZ(const ArithType &amount) {
            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                BasePoints[i].adjZ(amount);
            }
        }

        void  roll(                           const double &angle) {eulerRotate(EULER_ROLl , angle);}
        void pitch(                           const double &angle) {eulerRotate(EULER_PITCH, angle);}
        void   yaw(                           const double &angle) {eulerRotate(EULER_YAW  , angle);}
        void euler(const unsigned char &name, const double &angle) {eulerRotate(name, angle);}

        void project(const Coord3D<ArithType> &camPos) {
            const double focalLength = camPos.getY();

            for (unsigned long i = 0; i < BasePoints.size(); i++) {
                const Coord3D<ArithType> point = Origin + AlteredPoints[i] - Coord3D<ArithType>(camPos.getX(), 0, camPos.getZ());
            
                if (focalLength == 0) {continue;}

                ProjectedPoints[i].setX((focalLength * point.getX()) / focalLength);
                ProjectedPoints[i].setY((focalLength * point.getZ()) / focalLength);
            }
        }

        void draw(RenderWindow &window, const SDL_Color &color = DefaultColors[COLOR_WHITE]) {
            for (unsigned long i = 0; i < Pairs.size(); i++) {
                window.drawLine(
                    Origin.getX() + ProjectedPoints[Pairs[i].first ].getX(),
                    Origin.getZ() + ProjectedPoints[Pairs[i].first ].getY(),
                    Origin.getX() + ProjectedPoints[Pairs[i].second].getX(),
                    Origin.getZ() + ProjectedPoints[Pairs[i].second].getY(),
                    color
                );
            }
        }
};

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}

    RenderWindow Window("window", 1280, 720);
    SDL_Event Event;
    const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

    struct {
        SDL_Point Pos = {0, 0};
        SDL_Point Rel = {0, 0};
        double Sensitivity = 10;

        bool Motion = false;
        bool Released = true;
        bool Captured = false;
    } MouseInfo;

    struct {
        int Quit = SDL_SCANCODE_ESCAPE;
        int ToggleCapture = SDL_SCANCODE_F1;
    } Keybinds;

    long double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    long double newTime = 0.0;
    long double frameTime = 0.0;
    double accumulator = 0.0;

    bool running = true;
    while (running) {
        startTicks = SDL_GetTicks();
        newTime = HireTime_Sec();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            while (SDL_PollEvent(&Event)) {
                switch (Event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_MOUSEMOTION:
                        MouseInfo.Motion = true;
                        SDL_GetMouseState(&MouseInfo.Pos.x, &MouseInfo.Pos.y);
                        MouseInfo.Pos.x = MouseInfo.Pos.x - Window.getW_2();
                        MouseInfo.Pos.y = Window.getH() - MouseInfo.Pos.y - Window.getH_2();

                        if (!MouseInfo.Released) {
                            MouseInfo.Rel.x = Event.motion.xrel;
                            MouseInfo.Rel.y = -Event.motion.yrel;
                        } else {
                            MouseInfo.Released = false;
                            MouseInfo.Rel = {0, 0};
                        }
                        break;
                    case SDL_KEYDOWN:
                        if (Keystate[Keybinds.ToggleCapture]) {
                            MouseInfo.Captured = !MouseInfo.Captured;
                            if (MouseInfo.Captured) {
                                SDL_SetRelativeMouseMode(SDL_TRUE);
                                MouseInfo.Released = true;
                                MouseInfo.Rel = {0, 0};
                            } else {
                                SDL_SetRelativeMouseMode(SDL_FALSE);
                                Window.centerMouse();
                                MouseInfo.Pos = {0, 0};
                            }
                        }
                        break;
                    case SDL_WINDOWEVENT:
                        Window.handleEvent(Event.window);
                        break;
                }
                if (!running) {break;}
            }
            if (!running || Keystate[Keybinds.Quit]) {
                running = false;
                break;
            }

            t += dt;
            accumulator -= dt;
            MouseInfo.Motion = false;
        }
        if (!running) {break;}

        Window.clear();
        Window.show();

        frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / Window.getRefreshRate()) {
            SDL_Delay(1000 / Window.getRefreshRate() - frameTicks);
        }
    }

    SDL_Quit();
    return 0; 
}
