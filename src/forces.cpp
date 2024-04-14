#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Vector2D.hpp"
#include "RenderWindow.hpp"
#include "DefaultColors.hpp"

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

class Projectile {
    private:
        Coord2D<double> Position = Coord2D<double>(0, 0);
        Vector2D<double> Velocity = Vector2D<double>(0, 0);

        double Lifetime = 10;
    
    public:
        Projectile() {}
        Projectile(const Coord2D<double> &position, const Vector2D<double> &velocity) : Position(position), Velocity(velocity) {}
        Projectile(const Coord2D<double> &position) : Position(position) {}

        Coord2D<double>   getP() const {return Position;}
        double           getPX() const {return Position.getX();}
        double           getPY() const {return Position.getY();}

        Coord2D<double>   setP(const Coord2D<double> &position) {
            Coord2D<double> output = Position;
            Position = position;
            return output;
        }
        double           setPX(const double &x) {return Position.setX(x);}
        double           setPY(const double &y) {return Position.setY(y);}

        Vector2D<double>  getV() const {return Velocity;}
        double           getVX() const {return Velocity.getX();}
        double           getVY() const {return Velocity.getY();}
        double           getVA() const {return Velocity.getAngle();}

        int update(const double &dt) {
            Position.adjX(Velocity.getX() * dt);
            Position.adjY(Velocity.getY() * dt);
            Lifetime -= dt;

            if (Lifetime <= 0) {
                return 1;
            }
            return 0;
        }
};

class PhysicsObject2D {
    private:
        Coord2D<double> Position = Coord2D<double>(0, 0);
        Vector2D<double> Velocity = Vector2D<double>(0, 0);
        Vector2D<double> Acceleration = Vector2D<double>(0, 0);
        Vector2D<double> Force = Vector2D<double>(0, 0);
        
        double Mass = 10;
        double StaticFriction = 1;
        double KineticFriction = 0.8;

        static double g;
        
    public:
        PhysicsObject2D() {}
        PhysicsObject2D(const Coord2D<double> &position) : Position(position) {}

        Coord2D<double>   getP() const {return Position;}
        double           getPX() const {return Position.getX();}
        double           getPY() const {return Position.getY();}

        Coord2D<double>   setP(const Coord2D<double> &position) {
            Coord2D<double> output = Position;
            Position = position;
            return output;
        }
        double           setPX(const double &x) {return Position.setX(x);}
        double           setPY(const double &y) {return Position.setY(y);}

        Vector2D<double>  getV() const {return Velocity;}
        double           getVX() const {return Velocity.getX();}
        double           getVY() const {return Velocity.getY();}
        double           getVA() const {return Velocity.getAngle();}

        Vector2D<double>  getA() const {return Acceleration;}
        double           getAX() const {return Acceleration.getX();}
        double           getAY() const {return Acceleration.getY();}
        double           getAA() const {return Acceleration.getAngle();}

        Vector2D<double>  getF() const {return Force;}
        double           getFX() const {return Force.getX();}
        double           getFY() const {return Force.getY();}
        double           getFA() const {return Force.getAngle();}

        double            getM() const {return Mass;}
        static double     getg()       {return g;}

        Vector2D<double> applyForce(const Vector2D<double> &force) {
            Vector2D<double> output = Force;
            Force += force;
            return output;
        }

        void resetMotion() {
            Acceleration = Vector2D<double>(0, 0);
            Velocity = Vector2D<double>(0, 0);
        }

        void update(double dt) {
            // Apply friction to object
            if (Velocity.getMag() == 0 && Mass * g * StaticFriction < Force.getMag()) {Force.adjMag(-Mass * g * StaticFriction);}
            else if (Velocity.getMag() > 0) {applyForce(Vector2D<double>(Mass * g * KineticFriction, Velocity.getAngle() + M_PI, true));}

            // Update motion values
            Acceleration = Force / Mass;
            Velocity += Acceleration * dt;
            Position.adjX(Velocity.getX() * dt);
            Position.adjY(Velocity.getY() * dt);

            // Reset force
            Force = Vector2D<double>(0, 0);
        }
};
double PhysicsObject2D::g = 9.80655;

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}

    RenderWindow Window("forces, amiright?", 1280, 768, SDL_WINDOW_SHOWN);
    SDL_Event Event;
    const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

    SDL_Rect frame = {0, 0, 64, 64};
    Texture tile(Window.loadTexture("dev/thingy/gfx/tile.png"), frame);
    tile.setBlending(SDL_BLENDMODE_BLEND);
    tile.setOpacity(50);

    PhysicsObject2D Object(Coord2D<double>(-544, 0));
    Texture ObjectTexture(Window.loadTexture("dev/thingy/gfx/smile.png"), frame);
    ObjectTexture.setMods({255, 255, 0, 255});

    SDL_Point dst;
    std::vector<Projectile> projectiles;
    std::vector<Texture> projectileTextures;

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
        int Flap = SDL_SCANCODE_SPACE;
        int Up = SDL_SCANCODE_W;
        int Down = SDL_SCANCODE_S;
        int Left = SDL_SCANCODE_A;
        int Right = SDL_SCANCODE_D;
    } Keybinds;

    long double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    long double newTime = 0.0;
    long double frameTime = 0.0;
    double accumulator = 0.0;

    const int unitSize = 64;
    const double force = Object.getg() * Object.getM() + 50;
    const double strength = 200;
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
                        if (!Event.key.repeat) {
                            if (Keystate[Keybinds.Quit]) {
                                running = false;
                                break;
                            }
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
                            if (Keystate[Keybinds.Flap]) {Object.applyForce(Vector2D<double>(0, force * 500));}
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        switch (Event.button.button) {
                            case SDL_BUTTON_LEFT:
                                projectiles.emplace_back(Projectile(Object.getP(), Vector2D<double>(100, std::atan2(MouseInfo.Pos.y - Object.getPY(), MouseInfo.Pos.x - Object.getPX()), true)));
                                projectileTextures.emplace_back(Texture(Window.loadTexture("dev/thingy/gfx/arrow.png"), frame));
                                projectileTextures[projectileTextures.size() - 1].setAngle(projectiles.back().getVA());
                                break;
                            case SDL_BUTTON_MIDDLE:
                                Object.setP(Coord2D<double>((double)MouseInfo.Pos.x, (double)MouseInfo.Pos.y));
                                break;
                            case SDL_BUTTON_RIGHT:
                                Object.resetMotion();
                                break;
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
                        break;
                    case SDL_WINDOWEVENT:
                        Window.handleEvent(Event.window);
                        break;
                }
                if (!running) {break;}
            }
            if (!running) {break;}

            if (Keystate[Keybinds.Up   ]) {Object.applyForce(Vector2D<double>(     0,  force));}
            if (Keystate[Keybinds.Down ]) {Object.applyForce(Vector2D<double>(     0, -force));}
            if (Keystate[Keybinds.Left ]) {Object.applyForce(Vector2D<double>(-force,      0));}
            if (Keystate[Keybinds.Right]) {Object.applyForce(Vector2D<double>( force,      0));}

            // Force of Gravity
            // Object.applyForce(Vector2D<double>(0, -Object.getg() * Object.getM()));
            
            Object.update(dt);

            // Confine the fella to the screen by wrapping the edges of the screen
            if (Object.getPX() - ObjectTexture.getCenter().x >  Window.getW_2()) {Object.setPX(-Window.getW_2() - ObjectTexture.getCenter().x);}
            if (Object.getPX() + ObjectTexture.getCenter().x < -Window.getW_2()) {Object.setPX( Window.getW_2() + ObjectTexture.getCenter().x);}
            if (Object.getPY() - ObjectTexture.getCenter().y >  Window.getH_2()) {Object.setPY(-Window.getH_2() - ObjectTexture.getCenter().y);}
            if (Object.getPY() + ObjectTexture.getCenter().y < -Window.getH_2()) {Object.setPY( Window.getH_2() + ObjectTexture.getCenter().y);}

            // Update the projectiles
            if (projectiles.size() > 0) {
                for (unsigned long i = projectiles.size() - 1; i >= 0; --i) {
                    if (projectiles[i].update(dt) == 1) {
                        projectiles.erase(projectiles.begin() + i);
                        projectileTextures.erase(projectileTextures.begin() + i);
                    }

                    if (i == 0) {break;}
                }
            }

            t += dt;
            accumulator -= dt;
            MouseInfo.Motion = false;
        }
        if (!running) {break;}

        Window.clear();

        for (int i = 0; i < Window.getW() / unitSize; i++) {
            for (int j = 0; j < Window.getH() / unitSize; j++) {
                SDL_Point p = {-Window.getW_2() + i * unitSize + tile.getCenter().x, Window.getH_2() - j * unitSize - tile.getCenter().y};
                Window.renderTexture(tile, p);
            }
        }

        dst = {(int)Object.getPX(), (int)Object.getPY()};
        Window.renderTexture(ObjectTexture, dst);
        for (unsigned long i = 0; i < projectiles.size(); i++) {
            dst = {(int)projectiles.at(i).getPX(), (int)projectiles.at(i).getPY()};
            Window.renderTexture(projectileTextures.at(i), dst);
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
