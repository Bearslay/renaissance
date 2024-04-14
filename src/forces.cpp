#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Coord2D.hpp"
#include "Vector2D.hpp"
#include "RenderWindow.hpp"
#include "DefaultColors.hpp"

class PhysicsObject2D {
    private:
        Coord2D<double> Position = Coord2D<double>(0, 0);
        Vector2D<double> Velocity = Vector2D<double>(0, 0);
        Vector2D<double> Acceleration = Vector2D<double>(0, 0);
        Vector2D<double> Force = Vector2D<double>(0, 0);
        double Mass = 1;

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
            // Force += Vector2D<double>(0, -g) * dt;
            // Force -= Vector2D<double>(Mass * g * 0.05, Force.getAngle() + M_PI, true);
            Acceleration = Force / Mass;
            Velocity += Acceleration * dt;
            Position.adjX(Velocity.getX() * dt);
            Position.adjY(Velocity.getY() * dt);
            Force = Vector2D<double>(0, 0);
        }
};

double PhysicsObject2D::g = 1;

double HireTime_Sec() {return SDL_GetTicks() * 0.01f;}
int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";}
    if (!IMG_Init(IMG_INIT_PNG)) {std::cout << "Error initializing SDL2_image\nERROR: " << SDL_GetError() << "\n";}

    RenderWindow Window("forces, amiright?", 1280, 768, SDL_WINDOW_SHOWN);
    SDL_Event Event;
    const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

    SDL_Rect dst = {0, 0, 64, 64};
    Texture tile(Window.loadTexture("dev/thingy/gfx/tile.png"), dst);
    tile.setBlending(SDL_BLENDMODE_BLEND);
    tile.setOpacity(50);

    PhysicsObject2D Bird(Coord2D<double>(-544, 0));
    SDL_Point birdPos;
    Texture bird(Window.loadTexture("dev/thingy/gfx/smile.png"), dst);
    bird.setMods({255, 255, 0, 255});

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
    const double force = Bird.getg() * Bird.getM() + 10;

    // bool startedGame = false;

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
                            if (Keystate[Keybinds.Flap]) {Bird.applyForce(Vector2D<double>(0, force * 250));}
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        switch (Event.button.button) {
                            case SDL_BUTTON_LEFT:
                                Bird.setP(Coord2D<double>((double)MouseInfo.Pos.x, (double)MouseInfo.Pos.y));
                                break;
                            case SDL_BUTTON_MIDDLE:
                                
                                break;
                            case SDL_BUTTON_RIGHT:
                                Bird.resetMotion();
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
            // if (Keystate[Keybinds.Flap]) {
            //     if (!startedGame) {startedGame = true;}
            //     Bird.applyForce(Vector2D<double>(0, 0.5));
            // }

            // if (startedGame) {
            //     Bird.update(dt);
            // }

            if (Keystate[Keybinds.Up   ]) {Bird.applyForce(Vector2D<double>(     0,  force));}
            if (Keystate[Keybinds.Down ]) {Bird.applyForce(Vector2D<double>(     0, -force));}
            if (Keystate[Keybinds.Left ]) {Bird.applyForce(Vector2D<double>(-force,      0));}
            if (Keystate[Keybinds.Right]) {Bird.applyForce(Vector2D<double>( force,      0));}

            Bird.applyForce(Vector2D<double>(0, -Bird.getg() * Bird.getM()));
            Bird.update(dt);

            if (Bird.getPX() - bird.getCenter().x >  Window.getW_2()) {Bird.setPX(-Window.getW_2() - bird.getCenter().x);}
            if (Bird.getPX() + bird.getCenter().x < -Window.getW_2()) {Bird.setPX( Window.getW_2() + bird.getCenter().x);}
            if (Bird.getPY() - bird.getCenter().y >  Window.getH_2()) {Bird.setPY(-Window.getH_2() - bird.getCenter().y);}
            if (Bird.getPY() + bird.getCenter().y < -Window.getH_2()) {Bird.setPY( Window.getH_2() + bird.getCenter().y);}

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

        birdPos = {(int)Bird.getPX(), (int)Bird.getPY()};
        Window.renderTexture(bird, birdPos);

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
