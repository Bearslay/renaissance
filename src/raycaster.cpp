#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "btils.hpp"
#include "bengine.hpp"

class player_top_down {
    protected:
        bengine::basic_collider_2d collider = bengine::basic_collider_2d(0, 0, 0.5, 0.5);

        // \brief rotation of player (radians; 0 is to the right of the screen)
        double rotation = 0;
        // \brief speed of the player ([length-unit]s per [time-unit] | [m/s])
        double movespeed = 0.25;

    public:
        player_top_down() {}
        player_top_down(const double &x_pos, const double &y_pos, const double &rotation) {
            this->collider.set_x_pos(x_pos);
            this->collider.set_y_pos(y_pos);
            this->set_rotation(rotation);
        }

        double get_x_pos() const {
            return this->collider.get_x_pos() + this->collider.get_width() / 2;
        }
        double get_y_pos() const {
            return this->collider.get_y_pos() + this->collider.get_height() / 2;
        }
        double get_rotation() const {
            return this->rotation;
        }
        double get_movespeed() const {
            return this->movespeed;
        }
        double get_radius() const {
            return this->collider.get_width() / 2;
        }
        double get_diameter() const {
            return this->collider.get_width();
        }

        void set_x_pos(const double &x_pos) {
            this->collider.set_x_pos(x_pos - this->collider.get_width() / 2);
        }
        void set_y_pos(const double &y_pos) {
            this->collider.set_y_pos(y_pos - this->collider.get_height() / 2);
        }
        void set_rotation(const double &rotation) {
            this->rotation = btils::normalizeRadian(rotation);
        }
        void set_movespeed(const double &movespeed) {
            this->movespeed = movespeed < 0 ? 0 : movespeed;
        }
        void set_radius(const double &radius) {
            this->collider.set_width((radius <= 0 ? C_0_RIGHT : radius) * 2);
            this->collider.set_height(this->collider.get_width());
        }
        void set_diameter(const double &diameter) {
            this->collider.set_width(diameter <= 0 ? C_0_RIGHT * 2 : diameter);
            this->collider.set_height(this->collider.get_width());
        }

        void move_x(const double &amount) {
            this->set_x_pos(this->get_x_pos() + amount);
        }
        void move_y(const double &amount) {
            this->set_y_pos(this->get_y_pos() + amount);
        }

        void look_cw(const double &amount) {
            this->rotation = btils::normalizeRadian(this->rotation - amount);
        }
        void look_ccw(const double &amount) {
            this->rotation = btils::normalizeRadian(this->rotation + amount);
        }

        bool detect_collision(const bengine::basic_collider_2d &other) const {
            return this->collider.detect_collision(other);
        }

        bool fix_collision(bengine::basic_collider_2d &other, const bengine::basic_collider_2d::fix_mode &fix_mode, const bool &do_initial_collision_check = true) {
            return this->collider.fix_collision(other, fix_mode, do_initial_collision_check);
        }
};

class player_raycaster : public player_top_down {
    private:
        // \brief angle that represents the area that the player can see (radians)
        double fov = C_PI_2;
        // \brief how far one ray from the player will reach before expiring ([length-unit]s | [m])
        double view_distance = 5;
        // \brief how fast the player can look left/right (radians per second)
        double look_speed = C_PI_2;
        // \brief how fast the player can zoom in/out ([length-unit]s per second [m/s])
        double zoom_speed = 0.5;
    
    public:
        player_raycaster() : player_top_down() {}
        player_raycaster(const double &x_pos, const double &y_pos, const double &rotation) : player_top_down(x_pos, y_pos, rotation) {}
        player_raycaster(const double &x_pos, const double &y_pos, const double &rotation, const double &fov, const double &view_distance) : player_top_down(x_pos, y_pos, rotation) {
            this->set_fov(fov);
            this->set_view_distance(view_distance);
        }

        double get_fov() const {
            return this->fov;
        }
        double get_view_distance() const {
            return this->view_distance;
        }
        double get_look_speed() const {
            return this->look_speed;
        }
        double get_zoom_speed() const {
            return this->zoom_speed;
        }

        /** Set the FOV (field of view) of the player on the interval (0, 2*PI)
         * \param fov New FOV for the player
         */
        void set_fov(const double &fov) {
            this->fov = fov <= 0 ? C_0_RIGHT : (fov >= C_2PI ? C_2PI_LEFT : fov);
        }
        void set_view_distance(const double &view_distance) {
            this->view_distance = view_distance < 0 ? 0 : view_distance;
        }
        void set_look_speed(const double &look_speed) {
            this->look_speed = this->look_speed < 0 ? 0 : look_speed;
        }
        void set_zoom_speed(const double &zoom_speed) {
            this->zoom_speed = this->zoom_speed < 0 ? 0 : zoom_speed;
        }
};

class raycaster : public bengine::loop {
    private:
        struct {
            int quit = SDL_SCANCODE_ESCAPE;
            int move_forwards = SDL_SCANCODE_W;
            int move_backwards = SDL_SCANCODE_S;
            int strafe_left = SDL_SCANCODE_A;
            int strafe_right = SDL_SCANCODE_D;
            int look_left = SDL_SCANCODE_LEFT;
            int look_right = SDL_SCANCODE_RIGHT;
        } keybinds;

        bengine::basicTexture minimap_texture;
        TTF_Font *font = TTF_OpenFont("dev/fonts/GNU-Unifont.ttf", 20);

        std::vector<std::vector<Uint8>> grid;

        /** 8-bit bitmask containing settings for the minimap
         * 
         * Formatting:
         * `Unused__ | Corner__ | Enabled_`
         * `00000___ | _____00_ | _______0`
         * 
         * Corner:
         * 0 = Top-Left, 1 = Top-Right, 2 = Bottom-Left, 3 = Bottom-Right
         * 
         * Enabled:
         * 0 = False, 1 = True
         */
        Uint8 minimap_settings = 3;
        Uint8 minimap_cell_size = 16;

        player_raycaster player;
        player_raycaster minimap_player = player_raycaster(360 / 2, 360 / 2, this->player.get_rotation()); // Change 360 to a different variable later

        std::vector<bengine::basic_collider_2d> colliders;

        double calc_move_angle(const bool &f, const bool &b, const bool &l, const bool &r) {
            if (f && !b) {
                if (l && !r) {
                    return C_3PI_4;
                }
                if (!l && r) {
                    return C_PI_4;
                }
                return C_PI_2;
            }
            if (!f && b) {
                if (l && !r) {
                    return C_5PI_4;
                }
                if (!l && r) {
                    return C_7PI_4;
                }
                return C_3PI_2;
            }
            if (l && !r) {
                return C_PI;
            }
            if (!l && r) {
                return 0;
            }
            return -1;
        }

        void handleEvent() override {}
        void compute() override {
            if (this->keystate[this->keybinds.quit]) {
                this->loopRunning = false;
            }

            if (this->keystate[this->keybinds.move_forwards]) {
                player.move_y(-this->player.get_movespeed() * this->deltaTime);
                this->visualsChanged = true;
            }
            if (this->keystate[this->keybinds.move_backwards]) {
                player.move_y(this->player.get_movespeed() * this->deltaTime);
                this->visualsChanged = true;
            }
            if (this->keystate[this->keybinds.strafe_left]) {
                player.move_x(-this->player.get_movespeed() * this->deltaTime);
                this->visualsChanged = true;
            }
            if (this->keystate[this->keybinds.strafe_right]) {
                player.move_x(this->player.get_movespeed() * this->deltaTime);
                this->visualsChanged = true;
            }

            if (this->keystate[this->keybinds.look_left]) {
                player.look_ccw(this->player.get_look_speed() * this->deltaTime);
                this->visualsChanged = true;
            } else if (this->keystate[this->keybinds.look_right]) {
                player.look_cw(this->player.get_look_speed() * this->deltaTime);
                this->visualsChanged = true;
            }
            // if (keystate[SDL_SCANCODE_UP]) {
            //     player.zoomOut(zoomspeed);
            // } else if (keystate[SDL_SCANCODE_DOWN]) {
            //     player.zoomIn(zoomspeed);
            // }

            for (std::size_t i = 0; i < this->colliders.size(); i++) {
                this->player.fix_collision(this->colliders[i], bengine::basic_collider_2d::fix_mode::MOVE_SELF, true);
            }
        }

        void create_minimap_texture() {
            this->window.targetDummy();
            this->window.initDummy(this->grid.at(0).size() * minimap_cell_size, this->grid.size() * minimap_cell_size);
            this->window.clear();

            for (std::size_t row = 0; row < this->grid.size(); row++) {
                for (std::size_t col = 0; col < this->grid.at(0).size(); col++) {
                    if (grid.at(row).at(col) > 0) {
                        this->window.fillRectangle(col * minimap_cell_size, row * minimap_cell_size, minimap_cell_size, minimap_cell_size, bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::WHITE)]);
                    }
                }
            }
            this->window.present();

            this->minimap_texture.setTexture(this->window.copyDummy());
            this->window.targetWindow();
            this->window.clear();
        }
        void render() override {
            // Minimap rendering
            if ((Uint8)(this->minimap_settings << 7) >> 7 == 1) {
                const Uint16 minimap_side_length = 360;
                const Uint16 minimap_corner_offset = 32;
                const Uint16 minimap_x_pos = ((Uint8)(this->minimap_settings << 5) >> 6) % 2 == 0 ? minimap_corner_offset : this->window.getWidth() - minimap_side_length - minimap_corner_offset;
                const Uint16 minimap_y_pos = (Uint8)(this->minimap_settings << 5) >> 6 <= 1 ? minimap_corner_offset : this->window.getHeight() - minimap_side_length - minimap_corner_offset;
                
                const double view_distance = this->player.get_view_distance() * 2 > this->grid.size() || this->player.get_view_distance() * 2 > this->grid.at(0).size() ? std::min(this->grid.size(), this->grid.at(0).size()) / 2 : this->player.get_view_distance();
                const Uint16 minimap_view_x_pos = this->player.get_x_pos() - view_distance < 0 ? 0 : (this->player.get_x_pos() + view_distance > this->grid.at(0).size() ? (this->grid.at(0).size() - view_distance * 2) * this->minimap_cell_size : (this->player.get_x_pos() - view_distance) * this->minimap_cell_size);
                const Uint16 minimap_view_y_pos = this->player.get_y_pos() - view_distance < 0 ? 0 : (this->player.get_y_pos() + view_distance > this->grid.size() ? (this->grid.size() - view_distance * 2) * this->minimap_cell_size : (this->player.get_y_pos() - view_distance) * this->minimap_cell_size);
                const double minimap_scale_factor = minimap_side_length / (2 * view_distance * this->minimap_cell_size);

                if (this->player.get_x_pos() < view_distance) {
                    this->minimap_player.set_x_pos(this->player.get_x_pos() * minimap_scale_factor * this->minimap_cell_size);
                } else if (this->player.get_x_pos() > this->grid.at(0).size() - view_distance) {
                    this->minimap_player.set_x_pos(minimap_side_length - (this->grid.at(0).size() - this->player.get_x_pos()) * minimap_scale_factor * this->minimap_cell_size);
                }
                if (this->player.get_y_pos() < view_distance) {
                    this->minimap_player.set_y_pos(this->player.get_y_pos() * minimap_scale_factor * this->minimap_cell_size);
                } else if (this->player.get_y_pos() > this->grid.size() - view_distance) {
                    this->minimap_player.set_y_pos(minimap_side_length - (this->grid.size() - this->player.get_y_pos()) * minimap_scale_factor * this->minimap_cell_size);
                }

                this->window.fillRectangle(minimap_x_pos - minimap_side_length / 30, minimap_y_pos - minimap_side_length / 30, minimap_side_length + minimap_side_length / 15, minimap_side_length + minimap_side_length / 15, bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::DARK_GRAY)]);
                this->minimap_texture.setFrame({minimap_view_x_pos, minimap_view_y_pos, (int)(view_distance * this->minimap_cell_size * 2), (int)(view_distance * this->minimap_cell_size * 2)});
                this->window.renderBasicTexture(this->minimap_texture, {minimap_x_pos, minimap_y_pos, minimap_side_length, minimap_side_length});

                minimap_player.set_radius(this->player.get_radius() * (minimap_side_length / (2 * view_distance * this->minimap_cell_size)) * this->minimap_cell_size);
                this->window.fillRectangle(minimap_x_pos + minimap_player.get_x_pos() - minimap_player.get_radius(), minimap_y_pos + minimap_player.get_y_pos() - minimap_player.get_radius(), minimap_player.get_radius() * 2, minimap_player.get_radius() * 2, bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::RED)]);
            }

            // Temporary full-view of the map
            this->window.renderText(this->font, btils::to_u16string("(" + btils::toString<double>(this->player.get_x_pos()) + ", " + btils::toString<double>(this->player.get_y_pos()) + ")").c_str(), 0, 0);
            this->window.renderSDLTexture(this->minimap_texture.getTexture(), {0, 0, (int)(this->grid.at(0).size() * this->minimap_cell_size), (int)(this->grid.size() * this->minimap_cell_size)}, {50, 50, (int)(this->grid.at(0).size() * this->minimap_cell_size), (int)(this->grid.size() * this->minimap_cell_size)});
            this->window.fillRectangle(50 + (this->player.get_x_pos() - this->player.get_radius()) * this->minimap_cell_size, 50 + (this->player.get_y_pos() - this->player.get_radius()) * this->minimap_cell_size, this->player.get_radius() * this->minimap_cell_size * 2, this->player.get_radius() * this->minimap_cell_size * 2, bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::RED)]);
        }

    public:
        raycaster(const std::vector<std::vector<Uint8>> &grid) : bengine::loop("raycaster", 1280, 720, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/) {
            // in the case of an empty input grid, a 16x16 box is created as a "default"
            if (grid.empty()) {
                this->grid = {
                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
                };

                this->colliders.emplace_back(bengine::basic_collider_2d(0, 0, 16, 1));
                this->colliders.emplace_back(bengine::basic_collider_2d(0, 1, 1, 14));
                this->colliders.emplace_back(bengine::basic_collider_2d(15, 1, 1, 14));
                this->colliders.emplace_back(bengine::basic_collider_2d(0, 15, 16, 1));
                this->colliders.emplace_back(bengine::basic_collider_2d(7, 7, 2, 2));
                this->colliders.emplace_back(bengine::basic_collider_2d(2, 11, 2, 2));
                this->colliders.emplace_back(bengine::basic_collider_2d(12, 3, 1, 7));
            } else {
                std::size_t longest_row_cols = 0;
                for (std::size_t row = 0; row < grid.size(); row++) {
                    this->grid.emplace_back();
                    for (std::size_t col = 0; col < grid.at(row).size(); col++) {
                        this->grid[row].emplace_back(grid.at(row).at(col));
                    }
                    if (this->grid.at(row).size() > longest_row_cols) {
                        longest_row_cols = this->grid.at(row).size();
                    }
                }
                for (std::size_t row = 0; row < this->grid.size(); row++) {
                    for (std::size_t col = this->grid.at(row).size(); col < longest_row_cols; col++) {
                        this->grid[row].emplace_back(0);
                    }
                }
            }

            this->create_minimap_texture();
            this->player.set_x_pos(this->grid.at(0).size() / 2);
            this->player.set_y_pos(this->grid.size() / 2);
            this->player.set_movespeed(0.25);
        }
        ~raycaster() {
            TTF_CloseFont(this->font);
        }
};

int main(int argc, char* args[]) {
    std::vector<std::vector<Uint8>> grid = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,1,0,0,1,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,1},
        {1,0,0,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,1},
        {1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
        {1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,1},
        {1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1},
        {1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1},
        {1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    raycaster r({});
    return r.run();
}
