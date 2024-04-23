#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Vector2D.hpp"
#include "RenderWindow.hpp"
#include "BColors.hpp"

struct {
    double g = 9.80665;
} Constants;

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

    template <typename ArithType> Vector2D<ArithType> landingVector(const ArithType &mag, const double &angle, const ArithType &dy, const bool &useRadians = true) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        return Vector2D<ArithType>(std::is_integral<ArithType>::value ? std::round(mag * std::cos(angle * (useRadians ? 1 : M_PI / 180))) : (mag * std::cos(angle * (useRadians ? 1 : M_PI / 180))), mag * std::sin(angle * (useRadians ? 1 : M_PI / 180)) - Constants.g * airTime<double>(mag, angle, dy, useRadians), useRadians);
    }
}

template <typename Type> Type generalSet(Type &thing, const Type &newVal) {
    const Type output = thing;
    thing = newVal;
    return output;
}
template <typename Type> Type generalAdjust(Type &thing, const Type &amount) {
    const Type output = thing;
    thing += amount;
    return output;
}

class PhysicsObject2D {
    private:
        Coord2D<double> Position = Coord2D<double>(0, 0);
        Vector2D<double> Velocity = Vector2D<double>(0, 0);
        Vector2D<double> Acceleration = Vector2D<double>(0, 0);
        Vector2D<double> Force = Vector2D<double>(0, 0);
        
        double Mass = 10;
        double StaticFriction = 1;
        double KineticFriction = 0.8;
        
        bool ApplyFriction = false;
        bool ApplyGravity = false;
        static double GravityDirection;

    public:
        PhysicsObject2D() {}
        PhysicsObject2D(const Coord2D<double> &position) : Position(position) {}

        Coord2D<double>     getP() const {return Position;}
        double             getPx() const {return Position.getX();}
        double             getPy() const {return Position.getY();}
        Vector2D<double>    getV() const {return Velocity;}
        double             getVx() const {return Velocity.getX();}
        double             getVy() const {return Velocity.getY();}
        double             getVa() const {return Velocity.getAngle();}
        Vector2D<double>    getA() const {return Acceleration;}
        double             getAx() const {return Acceleration.getX();}
        double             getAy() const {return Acceleration.getY();}
        double             getAa() const {return Acceleration.getAngle();}
        Vector2D<double>    getF() const {return Force;}
        double             getFx() const {return Force.getX();}
        double             getFy() const {return Force.getY();}
        double             getFa() const {return Force.getAngle();}
        double           getMass() const {return Mass;}
        double             getUs() const {return StaticFriction;}
        double             getUk() const {return KineticFriction;}

        bool hasFriction() const {return ApplyFriction;}
        bool  hasGravity() const {return ApplyGravity;}
        static double getGravityDirection(const bool &useRadians = true) {return GravityDirection * (useRadians ? 1 : 180 / M_PI);}

        Coord2D<double>     setP(const Coord2D<double> &position)      {return generalSet<Coord2D<double>>(Position, position);}
        double             setPx(const double &x)                      {return Position.setX(x);}
        double             setPy(const double &y)                      {return Position.setY(y);}
        Vector2D<double>    setV(const Vector2D<double> &velocity)     {return generalSet<Vector2D<double>>(Velocity, velocity);}
        double             setVx(const double &x)                      {return Velocity.setX(        x);}
        double             setVy(const double &y)                      {return Velocity.setY(        y);}
        double             setVa(const double &angle)                  {return Velocity.setAngle(angle);}
        Vector2D<double>    setA(const Vector2D<double> &acceleration) {return generalSet<Vector2D<double>>(Acceleration, acceleration);}
        double             setAx(const double &x)                      {return Acceleration.setX(        x);}
        double             setAy(const double &y)                      {return Acceleration.setY(        y);}
        double             setAa(const double &angle)                  {return Acceleration.setAngle(angle);}
        Vector2D<double>    setF(const Vector2D<double> &force)        {return generalSet<Vector2D<double>>(Force, force);}
        double             setFx(const double &x)                      {return Force.setX(        x);}
        double             setFy(const double &y)                      {return Force.setY(        y);}
        double             setFa(const double &angle)                  {return Force.setAngle(angle);}
        double           setMass(const double &mass)                   {return generalSet<double>(           Mass,            mass);}
        double             setUs(const double &staticFriction)         {return generalSet<double>( StaticFriction,  staticFriction);}
        double             setUk(const double &kineticFriction)        {return generalSet<double>(KineticFriction, kineticFriction);}

        bool setFriction(const bool &state) {return generalSet<bool>(ApplyFriction, state);}
        bool  setGravity(const bool &state) {return generalSet<bool>( ApplyGravity, state);}
        static double setGravityDirection(const bool &amount) {return generalSet<double>(GravityDirection, amount);}

        Coord2D<double>     adjP(const Coord2D<double> &amount)  {return generalAdjust<Coord2D<double>>(Position, amount);}
        double             adjPx(const double &amount)           {return Position.adjX(amount);}
        double             adjPy(const double &amount)           {return Position.adjY(amount);}
        Vector2D<double>    adjV(const Vector2D<double> &amount) {return generalAdjust<Vector2D<double>>(Velocity, amount);}
        double             adjVx(const double &amount)           {return Velocity.adjX(    amount);}
        double             adjVy(const double &amount)           {return Velocity.adjY(    amount);}
        double             adjVa(const double &amount)           {return Velocity.adjAngle(amount);}
        Vector2D<double>    adjA(const Vector2D<double> &amount) {return generalAdjust<Vector2D<double>>(Acceleration, amount);}
        double             adjAx(const double &amount)           {return Acceleration.adjX(    amount);}
        double             adjAy(const double &amount)           {return Acceleration.adjY(    amount);}
        double             adjAa(const double &amount)           {return Acceleration.adjAngle(amount);}
        Vector2D<double>    adjF(const Vector2D<double> &amount) {return generalAdjust<Vector2D<double>>(Force, amount);}
        double             adjFx(const double &amount)           {return Force.adjX(    amount);}
        double             adjFy(const double &amount)           {return Force.adjY(    amount);}
        double             adjFa(const double &amount)           {return Force.adjAngle(amount);}
        double           adjMass(const double &amount)           {return generalAdjust<double>(           Mass, amount);}
        double             adjUs(const double &amount)           {return generalAdjust<double>( StaticFriction, amount);}
        double             adjUk(const double &amount)           {return generalAdjust<double>(KineticFriction, amount);}

        bool toggleFriction() {return generalSet<bool>(ApplyFriction, !ApplyFriction);}
        bool  toggleGravity() {return generalSet<bool>( ApplyGravity,  !ApplyGravity);}
        static double adjGravityDirection(const bool &amount) {return generalAdjust<double>(GravityDirection, amount);}

        void resetMotion() {
            Acceleration = Vector2D<double>(0, 0);
            Velocity = Vector2D<double>(0, 0);
        }

        int update(double dt) {
            // Apply friction to object
            if (ApplyFriction) {
                if (Velocity.getMag() == 0 && Mass * Constants.g * StaticFriction < Force.getMag()) {Force.adjMag(-Mass * Constants.g * StaticFriction);}
                else if (Velocity.getMag() > 0) {adjF(Vector2D<double>(Mass * Constants.g * KineticFriction, Velocity.getAngle() + M_PI, true));}
            }

            // Apply gravity
            if (ApplyGravity) {adjF(Vector2D<double>(Mass * Constants.g, GravityDirection, true));}

            // Update motion values
            Acceleration = Force / Mass;
            Velocity += Acceleration * dt;
            Position.adjX(Velocity.getX() * dt);
            Position.adjY(Velocity.getY() * dt);

            // Reset force
            Force = Vector2D<double>(0, 0);

            return 0;
        }
};
double PhysicsObject2D::GravityDirection = 3 * M_PI_2;

class Projectile2D : public PhysicsObject2D {
    private:
        double Lifetime = 100;
    
    public:
        Projectile2D(const Coord2D<double> &position, const Vector2D<double> &velocity) {
            PhysicsObject2D::setP(position);
            PhysicsObject2D::setV(velocity);
            PhysicsObject2D::setUs(0);
            PhysicsObject2D::setUk(0.05);
            PhysicsObject2D::setGravity(false);
        }
        Projectile2D(const Coord2D<double> &position) {Projectile2D(position, Vector2D<double>(0, 0));}
        Projectile2D() {Projectile2D(Coord2D<double>(0, 0), Vector2D<double>(0, 0));}

        int update(const double &dt) {
            PhysicsObject2D::update(dt);
            Lifetime -= dt;

            if (Lifetime <= 0) {
                return 1;
            }
            return 0;
        }
};

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}

    std::srand(time(NULL));

    RenderWindow Window("forces, amiright?", 1280, 768, SDL_WINDOW_SHOWN);
    SDL_Event Event;
    const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

    SDL_Rect frame = {0, 0, 64, 64};
    Texture tile(Window.loadTexture("dev/thingy/gfx/tile.png"), frame);
    tile.setBlending(SDL_BLENDMODE_BLEND);
    tile.setOpacity(50);

    PhysicsObject2D Object(Coord2D<double>(0, 0));
    Object.toggleFriction();
    Texture ObjectTexture(Window.loadTexture("dev/thingy/gfx/smile.png"), frame);
    ObjectTexture.setMods({255, 255, 0, 255});

    SDL_Point dst;
    std::vector<Projectile2D> projectiles;
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
        int Jump = SDL_SCANCODE_SPACE;
        int Up = SDL_SCANCODE_W;
        int Down = SDL_SCANCODE_S;
        int Left = SDL_SCANCODE_A;
        int Right = SDL_SCANCODE_D;
        int ToggleAutoPilot = SDL_SCANCODE_F2;
    } Keybinds;

    long double t = 0.0;
    double dt = 0.01;

    int startTicks = 0, frameTicks = 0;
    double currentTime = HireTime_Sec();
    long double newTime = 0.0;
    long double frameTime = 0.0;
    double accumulator = 0.0;

    const int unitSize = 64;
    const double force = Constants.g * Object.getMass() + 50;
    const double strength = 150;
    double throwAngle = 0;

    bool autoPilot = false;
    double moveAccumulator = 0;
    double shootAccumulator = 0;

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
                            if (Keystate[Keybinds.ToggleAutoPilot]) {
                                autoPilot = !autoPilot;
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
                            if (Keystate[Keybinds.Jump]) {Object.adjF(Vector2D<double>(0, force * 500));}
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        switch (Event.button.button) {
                            case SDL_BUTTON_LEFT:
                                // throwAngle = kmtcs::launchAngle(MouseInfo.Pos.x - Object.getPx(), MouseInfo.Pos.y - Object.getPy(), strength, true);
                                throwAngle = std::atan2(MouseInfo.Pos.y - Object.getPy(), MouseInfo.Pos.x - Object.getPx());
                                if (!std::isnan(throwAngle)) {
                                    projectiles.emplace_back(Projectile2D(Object.getP(), Vector2D<double>(strength, throwAngle, true)));
                                } else {
                                    projectiles.emplace_back(Projectile2D(Object.getP(), Vector2D<double>(strength, std::atan2(MouseInfo.Pos.y - Object.getPy(), MouseInfo.Pos.x - Object.getPx()), true)));
                                }
                                projectileTextures.emplace_back(Texture(Window.loadTexture("dev/thingy/gfx/arrow.png"), frame));
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

            if (autoPilot) {
                moveAccumulator += dt;
                shootAccumulator += dt;
                if (moveAccumulator >= 10) {
                    moveAccumulator = 0;
                    Object.adjF(Vector2D<double>(force * 350, std::rand() % 360, false));
                }
                if (shootAccumulator >= 1.5) {
                    shootAccumulator = 0;

                    throwAngle = kmtcs::launchAngle(MouseInfo.Pos.x - Object.getPx(), MouseInfo.Pos.y - Object.getPy(), strength, true);
                    if (!std::isnan(throwAngle)) {
                        projectiles.emplace_back(Projectile2D(Object.getP(), Vector2D<double>(strength, throwAngle, true)));
                    } else {
                        projectiles.emplace_back(Projectile2D(Object.getP(), Vector2D<double>(strength, std::atan2(MouseInfo.Pos.y - Object.getPy(), MouseInfo.Pos.x - Object.getPx()), true)));
                    }
                    projectileTextures.emplace_back(Texture(Window.loadTexture("dev/thingy/gfx/arrow.png"), frame));
                }
            }

            if (Keystate[Keybinds.Up   ]) {Object.adjF(Vector2D<double>(     0,  force));}
            if (Keystate[Keybinds.Down ]) {Object.adjF(Vector2D<double>(     0, -force));}
            if (Keystate[Keybinds.Left ]) {Object.adjF(Vector2D<double>(-force,      0));}
            if (Keystate[Keybinds.Right]) {Object.adjF(Vector2D<double>( force,      0));}

            // Object.adjF(Vector2D<double>(0, -Constants.g * Object.getMass()));            
            Object.update(dt);

            // Confine the fella to the screen by wrapping the edges of the screen
            if (Object.getPx() - ObjectTexture.getCenter().x >  Window.getW_2()) {Object.setPx(-Window.getW_2() - ObjectTexture.getCenter().x);}
            if (Object.getPx() + ObjectTexture.getCenter().x < -Window.getW_2()) {Object.setPx( Window.getW_2() + ObjectTexture.getCenter().x);}
            if (Object.getPy() - ObjectTexture.getCenter().y >  Window.getH_2()) {Object.setPy(-Window.getH_2() - ObjectTexture.getCenter().y);}
            if (Object.getPy() + ObjectTexture.getCenter().y < -Window.getH_2()) {Object.setPy( Window.getH_2() + ObjectTexture.getCenter().y);}

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

        dst = {(int)Object.getPx(), (int)Object.getPy()};
        Window.renderTexture(ObjectTexture, dst);
        for (unsigned long i = 0; i < projectiles.size(); i++) {
            dst = {(int)projectiles.at(i).getPx(), (int)projectiles.at(i).getPy()};
            projectileTextures[i].setAngle(projectiles[i].getVa());
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
