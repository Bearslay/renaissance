#include <iostream>
#include <vector>

#include "btils_string.hpp"
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
    switch (right + forwards * 2 + left * 4 + backwards * 8) {
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

class polynomial_function {
    private:
        std::vector<double> coefficients;

    public:
        polynomial_function() {
            this->coefficients = {0};
        }
        polynomial_function(const std::vector<double> &coefficients) {
            if (coefficients.size() == 0) {
                this->coefficients.emplace_back(0);
            }
            for (std::size_t i = 0; i < coefficients.size(); i++) {
                if (std::isnan(coefficients.at(i))) {
                    this->coefficients.emplace_back(0);
                    continue;
                }
                this->coefficients.emplace_back(coefficients.at(i));
            }
        }

        polynomial_function operator!() const {
            return *this * (-1);
        }
        polynomial_function operator+(const polynomial_function &rhs) const {
            const std::vector<double> &larger = this->coefficients.size() >= rhs.coefficients.size() ? this->coefficients : rhs.to_vector();
            const std::vector<double> &smaller = this->coefficients.size() < rhs.coefficients.size() ? this->coefficients : rhs.to_vector();

            std::vector<double> output = larger;
            for (std::size_t i = 0; i < smaller.size(); i++) {
                output[i] += smaller.at(i);
            }
            return polynomial_function(output);
        }
        polynomial_function operator-(const polynomial_function &rhs) const {
            const std::vector<double> &larger = this->coefficients.size() >= rhs.coefficients.size() ? this->coefficients : rhs.to_vector();
            const std::vector<double> &smaller = this->coefficients.size() < rhs.coefficients.size() ? this->coefficients : rhs.to_vector();

            std::vector<double> output = larger;
            for (std::size_t i = 0; i < smaller.size(); i++) {
                output[i] -= smaller.at(i);
            }
            return polynomial_function(output);
        }
        polynomial_function operator*(const polynomial_function &rhs) const {
            const std::vector<double> &larger = this->coefficients.size() >= rhs.coefficients.size() ? this->coefficients : rhs.to_vector();
            const std::vector<double> &smaller = this->coefficients.size() < rhs.coefficients.size() ? this->coefficients : rhs.to_vector();

            std::vector<double> output(larger.size() + smaller.size() - 1);
            for (std::size_t i = 0; i < smaller.size(); i++) {
                for (std::size_t j = 0; j < larger.size(); j++) {
                    output[i + j] += smaller[i] * larger[j];
                }
            }
            return polynomial_function(output);
        }
        polynomial_function operator*(const double &rhs) const {
            std::vector<double> output = this->coefficients;
            for (std::size_t i = 0; i < output.size(); i++) {
                output[i] *= rhs;
            }
            return polynomial_function(output);
        }

        polynomial_function& operator=(const polynomial_function &rhs) {
            this->coefficients = rhs.to_vector();
            return *this;
        }
        polynomial_function& operator+=(const polynomial_function &rhs) {
            *this = *this + rhs;
            return *this;
        }
        polynomial_function& operator-=(const polynomial_function &rhs) {
            *this = *this - rhs;
            return *this;
        }
        polynomial_function& operator*=(const polynomial_function &rhs) {
            *this = *this * rhs;
            return *this;
        }
        polynomial_function& operator*=(const double &rhs) {
            *this = *this * rhs;
            return *this;
        }

        bool operator==(const polynomial_function &rhs) const {
            return this->coefficients == rhs.to_vector();
        }
        bool operator!=(const polynomial_function &rhs) const {
            return !(*this == rhs);
        }

        double operator()(const double &x) const {
            double result = 0;
            for (std::size_t i = 0; i < this->coefficients.size(); i++) {
                result += this->coefficients.at(i) * std::pow(x, i);
            }
            return result;
        }

        double get_coefficient(const std::size_t &power) const {
            return power < this->coefficients.size() ? this->coefficients.at(power) : 0;
        }

        void set_coefficient(const std::size_t &power, const double &coefficient) {
            if (power < this->coefficients.size()) {
                this->coefficients[power] = coefficient;
                return;
            }

            for (std::size_t i = this->coefficients.size(); i < power; i++) {
                this->coefficients.emplace_back(0);
            }
            this->coefficients.emplace_back(coefficient);
        }

        std::vector<double> to_vector() const {
            return this->coefficients;
        }

        std::string to_string() const {
            std::string output = btils::toString<double>(this->coefficients.at(0));
            for (std::size_t i = 1; i < this->coefficients.size(); i++) {
                if (this->coefficients.at(i) == 0) {
                    continue;
                }
                if (std::fabs(this->coefficients.at(i)) == 1) {
                    output += (this->coefficients.at(i) < 0 ? " - x^" : " + x^") + btils::toString<std::size_t>(i);
                    continue;
                }
                output += (this->coefficients.at(i) < 0 ? " - " : " + ") + btils::toString<double>(std::fabs(this->coefficients.at(i))) + "x^" + btils::toString<std::size_t>(i);
            }
            return output + "\n";
        }
};

class projectile_and_movement_demo : public bengine::loop {
    private:
        bengine::normalMouseState mstate;

        struct {
            int move_forwards = SDL_SCANCODE_W;
            int move_backwards = SDL_SCANCODE_S;
            int strafe_left = SDL_SCANCODE_A;
            int strafe_right = SDL_SCANCODE_D;
            int orbit_reference_ccw = SDL_SCANCODE_LEFT;
            int orbit_reference_cw = SDL_SCANCODE_RIGHT;
            int orbit_mouse_ccw = SDL_SCANCODE_UP;
            int orbit_mouse_cw = SDL_SCANCODE_DOWN;
        } keybinds;

        bengine::moddedTexture fella_texture = bengine::moddedTexture(this->window.loadTexture("dev/thingy/gfx/smile.png"), {0, 0, 64, 64}, {255, 0, 0, 255});
        bengine::clickRectangle fella_box;
        bengine::basicTexture tile = bengine::basicTexture(this->window.loadTexture("dev/thingy/gfx/tile.png"), {0, 0, 64, 64});

        bengine::coordinate_2d<double> fella_position;
        double fella_speed = 0.25;
        double fella_strength = 150;
        double fella_radius = 16;

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
            const double movement_angle = get_movement_angle(this->keystate[this->keybinds.move_forwards], this->keystate[this->keybinds.move_backwards], this->keystate[this->keybinds.strafe_left], this->keystate[this->keybinds.strafe_right]);
            if (movement_angle >= 0) {
                this->fella_position += bengine::coordinate_2d<double>(std::cos(movement_angle), std::sin(-movement_angle)) * this->fella_speed;
                this->visualsChanged = true;
            }

            if (this->fella_position.get_euclidean_distance() > 0) {
                if (this->keystate[this->keybinds.orbit_reference_cw]) {
                    this->fella_position.rotate_about_reference_point(this->fella_speed / this->fella_position.get_euclidean_distance());    // Angle is opposite sign of what it should be due to left-handed coordinate system used
                    this->visualsChanged = true;
                } else if (this->keystate[this->keybinds.orbit_reference_ccw]) {
                    this->fella_position.rotate_about_reference_point(-this->fella_speed / this->fella_position.get_euclidean_distance());    // Angle is opposite sign of what it should be due to left-handed coordinate system used
                    this->visualsChanged = true;
                }
            }

            const double distance_from_mouse = std::sqrt((this->mstate.posx() - this->fella_position.get_x_pos()) * (this->mstate.posx() - this->fella_position.get_x_pos()) + (this->mstate.posy() - this->fella_position.get_y_pos()) * (this->mstate.posy() - this->fella_position.get_y_pos()));
            if (distance_from_mouse > 0) {
                if (this->keystate[this->keybinds.orbit_mouse_cw]) {
                    this->fella_position.rotate_about_pivot(bengine::coordinate_2d<double>(this->mstate.posx(), this->mstate.posy()), this->fella_speed / distance_from_mouse);    // Angle is opposite sign of what it should be due to left-handed coordinate system used
                    this->visualsChanged = true;
                } else if (this->keystate[this->keybinds.orbit_mouse_ccw]) {
                    this->fella_position.rotate_about_pivot(bengine::coordinate_2d<double>(this->mstate.posx(), this->mstate.posy()), -this->fella_speed / distance_from_mouse);    // Angle is opposite sign of what it should be due to left-handed coordinate system used
                    this->visualsChanged = true;
                }
            }

            if (this->fella_box.checkPos(this->mstate)) {
                this->fella_texture.setColorMod(bengine::window::getColorFromPreset(bengine::window::presetColor::YELLOW));
                this->visualsChanged = true;
            } else {
                this->fella_texture.setColorMod(bengine::window::getColorFromPreset(bengine::window::presetColor::RED));
                this->visualsChanged = true;
            }

            if (visualsChanged) {
                this->fella_box = bengine::clickRectangle(this->fella_position.get_x_pos() - this->fella_radius, this->fella_position.get_y_pos() - this->fella_radius, this->fella_position.get_x_pos() + this->fella_radius, this->fella_position.get_y_pos() + this->fella_radius);
            }
            this->mstate.stopMotion();
        }
        void render() override {
            const double launch_angle = bengine::kinematics_helper::launchAngle(this->fella_strength, this->mstate.posx() - this->fella_position.get_x_pos(), this->mstate.posy() - this->fella_position.get_y_pos(), true);
            if (!std::isnan(launch_angle)) {
                const double projectile_x_component = this->fella_strength * std::cos(launch_angle);

                const double dx = 0.05;
                double current_x = 0.0;
                double current_y = 0.0;
                double new_x = 0.0;
                double new_y = 0.0;

                polynomial_function equation = polynomial_function({0, this->fella_strength * std::sin(launch_angle), -0.5 * bengine::kinematics_helper::get_gravitational_constant()});

                for (double i = 0.0; i < (this->mstate.posx() - this->fella_position.get_x_pos()) / projectile_x_component; i += dx) {
                    new_x += dx;
                    new_y = equation(new_x);
                    this->window.drawLine(this->fella_position.get_x_pos() + current_x * projectile_x_component, this->fella_position.get_y_pos() - current_y, this->fella_position.get_x_pos() + new_x * projectile_x_component, this->fella_position.get_y_pos() - new_y, bengine::window::getColorFromPreset(bengine::window::presetColor::RED));
                    current_x = new_x;
                    current_y = new_y;
                }
            }
            this->window.renderModdedTexture(this->fella_texture, {this->fella_box.getX1(), this->fella_box.getY1(), this->fella_box.getW(), this->fella_box.getH()});
        }
    
    public:
        projectile_and_movement_demo() : bengine::loop("Projectile Trajectory and Movement Demo", 1280, 720, SDL_WINDOW_SHOWN, IMG_INIT_PNG, false) {
            bengine::coordinate_2d<double>::set_reference_point(bengine::coordinate_2d<double>(this->window.getWidth() / 2, this->window.getHeight() / 2));
            this->fella_position = bengine::coordinate_2d<double>::get_reference_point();
            this->fella_box = bengine::clickRectangle(this->fella_position.get_x_pos() - this->fella_radius, this->fella_position.get_y_pos() - this->fella_radius, this->fella_position.get_x_pos() + this->fella_radius, this->fella_position.get_y_pos() + this->fella_radius);
        }
};

int main() {
    projectile_and_movement_demo p;
    return p.run();

    polynomial_function p1 = polynomial_function({8, 4, -3, 1});
    polynomial_function p2 = polynomial_function({-1, 2, 1});
    std::cout << p1.to_string();
    std::cout << p2.to_string();

    p1 *= p2;
    std::cout << p1.to_string();
    std::cout << p2.to_string();

    p1 *= p2;
    std::cout << p1.to_string();
    std::cout << p2.to_string();

    // throwAngle = kmtcs::launchAngle<double>(MousePos.x - pos.getX(), MousePos.y - pos.getY(), strength, true);
    // if (!std::isnan(throwAngle)) {
    //     strengthComps = breakVector<double>(strength, throwAngle);

    //     double dx = 0.05;
    //     double cx = 0;
    //     double cy = 0;
    //     double nx = 0;
    //     double ny = 0;
    //     for (double i = 0; i < (MousePos.x - pos.getX()) / strengthComps.first; i += dx) {
    //         nx += dx;
    //         ny = -0.5 * Constants.g * nx * nx + strengthComps.second * nx;
    //         Window.drawLine(dst.x + cx * strengthComps.first, dst.y + cy, dst.x + nx * strengthComps.first, dst.y + ny, PresetColors[COLOR_RED]);
    //         cx = nx;
    //         cy = ny;
    //     }
    // }       

    return 0; 
}
