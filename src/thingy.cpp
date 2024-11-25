#include <iostream>
#include <vector>

#include "btils.hpp"
#include "bengine.hpp"

#include "bengine_coordinate_2d.hpp"

double getMoveAngle(const bool &f, const bool &b, const bool &l, const bool &r) {
    if (f && !b) {
        if (l && !r) {
            return 3 * M_PI_4;
        }
        if (!l && r) {
            return M_PI_4;
        }
        return M_PI_2;
    }
    if (!f && b) {
        if (l && !r) {
            return 5 * M_PI_4;
        }
        if (!l && r) {
            return 7 * M_PI_4;
        }
        return 3 * M_PI_2;
    }
    if (l && !r) {
        return M_PI;
    }
    if (!l && r) {
        return 0;
    }
    return -1;
}

enum class movement_direction : unsigned char {
    NONE = 0,
    RIGHT = 1,
    UP = 2,
    UP_RIGHT = 3,
    LEFT = 4,
    UP_LEFT = 6,
    DOWN = 8,
    DOWN_RIGHT = 9,
    DOWN_LEFT = 12
};
double get_movement_angle(const movement_direction &direction) {
    switch (direction) {
        default:
        case movement_direction::NONE:
            return -1;
        case movement_direction::RIGHT:
            return 0;
        case movement_direction::UP:
            return M_PI_2;
        case movement_direction::UP_RIGHT:
            return M_PI_4;
        case movement_direction::LEFT:
            return M_PI;
        case movement_direction::UP_LEFT:
            return M_PI_2 + M_PI_4;
        case movement_direction::DOWN:
            return M_PI + M_PI_2;
        case movement_direction::DOWN_RIGHT:
            return M_PI + M_PI_2 + M_PI_4;
        case movement_direction::DOWN_LEFT:
            return M_PI + M_PI_4;
    }
}
double get_movement_angle(const bool &forwards, const bool &backwards, const bool &left, const bool &right) {
    switch (forwards + backwards * 2 + left * 4 + right * 8) {
        case 1:
        case 11:
            return 0;
        case 2:
        case 7:
            return M_PI_2;
        case 3:
            return M_PI_4;
        case 4:
        case 14:
            return M_PI;
        case 6:
            return M_PI_2 + M_PI_4;
        case 8:
        case 13:
            return M_PI + M_PI_2;
        case 9:
            return M_PI + M_PI_2 + M_PI_4;
        case 12:
            return M_PI + M_PI_4;
        default:
            return -1;
    }
}

class projectile_and_movement_demo : public bengine::loop {
    private:
        bengine::normalMouseState mstate;

        struct {
            int moveForwards = SDL_SCANCODE_W;
            int moveBackwards = SDL_SCANCODE_S;
            int strafeLeft = SDL_SCANCODE_A;
            int strafeRight = SDL_SCANCODE_D;
        } keybinds;

        bengine::basicTexture fella = bengine::basicTexture(this->window.loadTexture("dev/thingy/gfx/smile.png"), {0, 0, 64, 64});
        bengine::basicTexture tile = bengine::basicTexture(this->window.loadTexture("dev/thingy/gfx/tile.png"), {0, 0, 64, 64});

        bengine::coordinate_2d<double> character_position = bengine::coordinate_2d<double>(1280 / 2, 720 / 2);
        double character_movespeed = 0.25;
        double character_strength = 150;

        void handleEvent() override {
            switch (this->event.type) {
                case SDL_MOUSEMOTION:
                    this->mstate.updateMotion(this->event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    this->mstate.pressButton(this->event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    this->mstate.releaseButton(this->event);
                    break;
            }
        }
        void compute() override {
            this->mstate.stopMotion();
        }
        void render() override {

        }
    
    public:
        projectile_and_movement_demo() : bengine::loop("Projectile Trajectory and Movement Demo", 1280, 720, SDL_WINDOW_SHOWN, IMG_INIT_PNG, false) {}
};

int main() {
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

        if (dstSize.x / 2 + 4 >= std::sqrt((MousePos.x - pos.getX()) * (MousePos.x - pos.getX()) + (MousePos.y - pos.getY()) * (MousePos.y - pos.getY()))) {texture.setMods(PresetColors[COLOR_YELLOW]);}
        else {texture.setMods(PresetColors[COLOR_RED]);}

        dst = {(int)pos.getX(), (int)pos.getY(), dstSize.x, dstSize.y};
        // Window.drawCircle(0, 0, pos.distEuclid(), keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT] ? PresetColors[COLOR_LIME] : PresetColors[COLOR_WHITE]);
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
                Window.drawLine(dst.x + cx * strengthComps.first, dst.y + cy, dst.x + nx * strengthComps.first, dst.y + ny, PresetColors[COLOR_RED]);
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
