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
            this->rotation = btils::normalize_radian_angle(rotation);
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
            this->rotation = btils::normalize_radian_angle(this->rotation - amount);
        }
        void look_ccw(const double &amount) {
            this->rotation = btils::normalize_radian_angle(this->rotation + amount);
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

        bengine::basic_texture minimap_texture;
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

        void handle_event() override {}
        void compute() override {
            if (this->keystate[this->keybinds.quit]) {
                this->loop_running = false;
            }

            if (this->keystate[this->keybinds.move_forwards]) {
                player.move_y(-this->player.get_movespeed() * this->delta_time);
                this->visuals_changed = true;
            }
            if (this->keystate[this->keybinds.move_backwards]) {
                player.move_y(this->player.get_movespeed() * this->delta_time);
                this->visuals_changed = true;
            }
            if (this->keystate[this->keybinds.strafe_left]) {
                player.move_x(-this->player.get_movespeed() * this->delta_time);
                this->visuals_changed = true;
            }
            if (this->keystate[this->keybinds.strafe_right]) {
                player.move_x(this->player.get_movespeed() * this->delta_time);
                this->visuals_changed = true;
            }

            if (this->keystate[this->keybinds.look_left]) {
                player.look_ccw(this->player.get_look_speed() * this->delta_time);
                this->visuals_changed = true;
            } else if (this->keystate[this->keybinds.look_right]) {
                player.look_cw(this->player.get_look_speed() * this->delta_time);
                this->visuals_changed = true;
            }
            // if (keystate[SDL_SCANCODE_UP]) {
            //     player.zoomOut(zoomspeed);
            // } else if (keystate[SDL_SCANCODE_DOWN]) {
            //     player.zoomIn(zoomspeed);
            // }

            for (std::size_t i = 0; i < this->colliders.size(); i++) {
                if (this->player.fix_collision(this->colliders[i], bengine::basic_collider_2d::fix_mode::MOVE_SELF, true)) {
                    this->visuals_changed = true;
                }
            }
        }

        void create_minimap_texture() {
            this->window.target_renderer_at_dummy();
            this->window.initialize_dummy(this->grid.at(0).size() * minimap_cell_size, this->grid.size() * minimap_cell_size);
            this->window.clear_renderer();

            for (std::size_t row = 0; row < this->grid.size(); row++) {
                for (std::size_t col = 0; col < this->grid.at(0).size(); col++) {
                    if (grid.at(row).at(col) > 0) {
                        this->window.fill_rectangle(col * minimap_cell_size, row * minimap_cell_size, minimap_cell_size, minimap_cell_size, bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE));
                    }
                }
            }
            this->window.present_renderer();

            this->minimap_texture.set_texture(this->window.duplicate_dummy());
            this->window.target_renderer_at_window();
            this->window.clear_renderer();
        }
        void render() override {
            // Minimap rendering
            if ((Uint8)(this->minimap_settings << 7) >> 7 == 1) {
                const Uint16 minimap_side_length = 360;
                const Uint16 minimap_corner_offset = 32;
                const Uint16 minimap_x_pos = ((Uint8)(this->minimap_settings << 5) >> 6) % 2 == 0 ? minimap_corner_offset : this->window.get_width() - minimap_side_length - minimap_corner_offset;
                const Uint16 minimap_y_pos = (Uint8)(this->minimap_settings << 5) >> 6 <= 1 ? minimap_corner_offset : this->window.get_height() - minimap_side_length - minimap_corner_offset;
                
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

                this->window.fill_rectangle(minimap_x_pos - minimap_side_length / 30, minimap_y_pos - minimap_side_length / 30, minimap_side_length + minimap_side_length / 15, minimap_side_length + minimap_side_length / 15, bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::DARK_GRAY));
                this->minimap_texture.set_frame({minimap_view_x_pos, minimap_view_y_pos, (int)(view_distance * this->minimap_cell_size * 2), (int)(view_distance * this->minimap_cell_size * 2)});
                this->window.render_basic_texture(this->minimap_texture, {minimap_x_pos, minimap_y_pos, minimap_side_length, minimap_side_length});

                minimap_player.set_radius(this->player.get_radius() * (minimap_side_length / (2 * view_distance * this->minimap_cell_size)) * this->minimap_cell_size);
                this->window.fill_rectangle(minimap_x_pos + minimap_player.get_x_pos() - minimap_player.get_radius(), minimap_y_pos + minimap_player.get_y_pos() - minimap_player.get_radius(), minimap_player.get_radius() * 2, minimap_player.get_radius() * 2, bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::RED));
            }

            // Temporary full-view of the map
            this->window.render_text(this->font, btils::to_u16string("(" + btils::to_string<double>(this->player.get_x_pos()) + ", " + btils::to_string<double>(this->player.get_y_pos()) + ")").c_str(), 0, 0);
            this->window.render_SDLTexture(this->minimap_texture.get_texture(), {0, 0, (int)(this->grid.at(0).size() * this->minimap_cell_size), (int)(this->grid.size() * this->minimap_cell_size)}, {50, 50, (int)(this->grid.at(0).size() * this->minimap_cell_size), (int)(this->grid.size() * this->minimap_cell_size)});
            this->window.fill_rectangle(50 + (this->player.get_x_pos() - this->player.get_radius()) * this->minimap_cell_size, 50 + (this->player.get_y_pos() - this->player.get_radius()) * this->minimap_cell_size, this->player.get_radius() * this->minimap_cell_size * 2, this->player.get_radius() * this->minimap_cell_size * 2, bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::RED));
        
            // Tempoarary viewing of the colliders
            for (std::size_t i = 0; i < this->colliders.size(); i++) {
                this->window.draw_rectangle(51 + this->colliders.at(i).get_x_pos() * this->minimap_cell_size, 51 + this->colliders.at(i).get_y_pos() * this->minimap_cell_size, this->colliders.at(i).get_width() * this->minimap_cell_size - 2, this->colliders.at(i).get_height() * this->minimap_cell_size - 2, {255, 0, 0, 255});
            }
        }

    public:
        raycaster(const std::vector<std::vector<Uint8>> &grid) : bengine::loop("raycaster", 1280, 720, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/) {
            // in the case of an empty input grid, a 16x16 box is created as a "default"
            if (grid.empty()) {
                this->grid = {
                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
                };
                this->colliders.emplace_back(bengine::basic_collider_2d(0, 0, 16, 1));
                this->colliders.emplace_back(bengine::basic_collider_2d(0, 1, 1, 15));
                this->colliders.emplace_back(bengine::basic_collider_2d(15, 1, 1, 14));
                this->colliders.emplace_back(bengine::basic_collider_2d(1, 15, 15, 1));
            } else {
                // Basic copying of an input vector to and output, but also ensures that the output is rectangular
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
                
                // Algorithm to generate colliders with, making sure that any colliders that can be merged are merged

                // Create a grid that will hold whether a cell has been visited or not
                std::vector<std::vector<bool>> visit_grid(this->grid.size(), std::vector<bool>(this->grid.at(0).size(), false));
                for (std::size_t row = 0; row < this->grid.size(); row++) {
                    for (std::size_t col = 0; col < this->grid.at(0).size(); col++) {
                        if (this->grid.at(row).at(col) == 0) {
                            visit_grid[row][col] = true;
                        }
                    }
                }

                std::size_t row_start = 0, col_start = 0;
                while (row_start < this->grid.size() && col_start < this->grid.at(0).size()) {
                    std::size_t row_end = row_start, col_end = col_start;
                    // First, start by going to the right until reached a cell visited before (which either means that its blank or has been used already; it can't be included in either case)
                    while (col_end < this->grid.at(0).size()) {
                        if (visit_grid.at(row_end).at(col_end)) {
                            break;
                        }
                        visit_grid[row_end][col_end] = true;
                        col_end++;
                    }
                    // Now the mesh has a width, so now we go down with that width until a row has a cell that has been visited before
                    while (row_end < this->grid.size() - 1) {
                        row_end++;

                        // Check to see if the next row is allowed to be added to the mesh
                        bool valid_row = true;
                        for (std::size_t col = col_start; col < col_end; col++) {
                            if (visit_grid.at(row_end).at(col)) {
                                valid_row = false;
                                break;
                            }
                        }

                        // If the new row isn't valid, then the row_end is retracted back to a valid row and the start/end points of the mesh are defined
                        if (!valid_row) {
                            row_end--;
                            break;
                        }
                        // If the new row is valid, then the entire row is marked as visited as it will be consumed by the mesh
                        for (std::size_t i = col_start; i < col_end; i++) {
                            visit_grid[row_end][i] = true;
                        }
                    }

                    // At this point, there should be the top-left and bottom-right corners of a new mesh defined
                    // Here, a new collider is added based upon the start/end positions of the mesh
                    this->colliders.emplace_back(bengine::basic_collider_2d(col_start, row_start, col_end - col_start, row_end - row_start + 1));

                    // Now, the new starting row/column is found by searching for the next spot that is unvisited
                    bool found_unvisited_cell = false;
                    while (row_start < this->grid.size()) {
                        while (col_start < this->grid.at(0).size()) {
                            if (!visit_grid.at(row_start).at(col_start)) {
                                found_unvisited_cell = true;
                                break;
                            }
                            col_start++;
                        }
                        if (found_unvisited_cell) {
                            break;
                        }
                        row_start++;
                        col_start = 0;
                        found_unvisited_cell = false;
                    }
                    if (!found_unvisited_cell) {
                        break;
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
    raycaster r(grid);
    return r.run();
}
