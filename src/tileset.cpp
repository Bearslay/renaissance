#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "btils.hpp"
#include "bengine.hpp"

class autotiler_demo : public bengine::loop {
    private:
        bengine::generic_mouse_state mstate;
        SDL_Point mouse_pos_grid = {0, 0};
        SDL_Point prev_mouse_pos_grid = {0, 0};

        unsigned char tileset_number = 0;
        std::vector<SDL_Texture*> tileset_textures = {
            window.load_texture("dev/png/imperialPath/sheet4bit.png"),
            window.load_texture("dev/png/imperialPath/sheet8bit.png"),
            window.load_texture("dev/png/ironFence/sheet4bit.png"),
            window.load_texture("dev/png/ironFence/sheet8bit.png")
        };

        std::vector<std::vector<std::vector<char>>> grid;
        Uint16 cell_size = 40;

        bengine::autotiler tiler;

        void handle_event() override {
            switch (this->event.type) {
                case SDL_MOUSEMOTION:
                    this->mstate.update_motion(this->event);
                    this->mouse_pos_grid = {this->mstate.get_x_pos() / this->cell_size, this->mstate.get_y_pos() / this->cell_size};
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    this->mstate.press_button(this->event);
                    if (this->mstate.pressed(bengine::generic_mouse_state::button_names::LEFT_MOUSE_BUTTON) || this->mstate.pressed(bengine::generic_mouse_state::button_names::RIGHT_MOUSE_BUTTON)) {
                        if (this->tileset_number % 2 == 0) {
                            this->tiler.modify_4_bit_grid(grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->event.button.button == SDL_BUTTON_LEFT, false);
                        } else {
                            this->tiler.modify_8_bit_grid(grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->event.button.button == SDL_BUTTON_LEFT, false);
                        }
                        this->visuals_changed = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    this->mstate.release_button(this->event);
                    break;
                case SDL_KEYDOWN:
                    if (!this->event.key.repeat) {
                        if (this->keystate[SDL_SCANCODE_SPACE]) {
                            this->tileset_number++;
                            this->tileset_number %= this->tileset_textures.size();
                        }
                        if (this->keystate[SDL_SCANCODE_C]) {
                            for (std::size_t i = 0; i < this->grid.at(0).size(); i++) {
                                for (std::size_t j = 0; j < this->grid.at(0).at(0).size(); j++) {
                                    this->grid[this->tileset_number][i][j] = -1;
                                }
                            }
                            this->visuals_changed = true;
                        }
                    }
                    break;
            }
        }
        void compute() override {
            if (this->mouse_pos_grid.x != this->prev_mouse_pos_grid.x || this->mouse_pos_grid.y != this->prev_mouse_pos_grid.y) {
                this->prev_mouse_pos_grid = this->mouse_pos_grid;

                if (this->mstate.pressed(bengine::generic_mouse_state::button_names::LEFT_MOUSE_BUTTON) || this->mstate.pressed(bengine::generic_mouse_state::button_names::RIGHT_MOUSE_BUTTON)) {
                    if (this->tileset_number % 2 == 0) {
                        this->tiler.modify_4_bit_grid(this->grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->mstate.pressed(bengine::generic_mouse_state::button_names::LEFT_MOUSE_BUTTON), false);
                    } else {
                        this->tiler.modify_8_bit_grid(this->grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->mstate.pressed(bengine::generic_mouse_state::button_names::LEFT_MOUSE_BUTTON), false);
                    }
                    this->visuals_changed = true;
                }
            }

            this->mstate.stop_motion();
        }
        void render() override {
            // Background stuff
            this->window.fill_rectangle(0, 0, this->window.get_width(), this->window.get_height(), bengine::render_window::preset_colors[static_cast<Uint8>(bengine::render_window::preset_color::WHITE)]);
            for (std::size_t i = 0; i < this->grid.at(0).size(); i++) {
                for (std::size_t j = 0; j < this->grid.at(0).at(i).size(); j++) {
                    this->window.draw_rectangle(j * this->cell_size, i * this->cell_size, this->cell_size, this->cell_size, bengine::render_window::preset_colors[static_cast<Uint8>(bengine::render_window::preset_color::BLACK)]);
                }
            }

            for (std::size_t i = 0; i < this->tileset_textures.size(); i++) {
                for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                    for (std::size_t k = 0; k < grid.at(i).at(j).size(); k++) {
                        if (grid.at(i).at(j).at(k) >= 0) {
                            this->window.render_SDLTexture(this->tileset_textures.at(i), {this->grid.at(i).at(j).at(k) % (i % 2 == 0 ? 4 : 8) * 16, this->grid.at(i).at(j).at(k) / (i % 2 == 0 ? 4 : 8) * 16, 16, 16}, {(int)(k * this->cell_size), (int)(j * this->cell_size), this->cell_size, this->cell_size});
                        }
                    }
                }
            }
        }

    public:
        autotiler_demo() : bengine::loop("Autotiler Demo", 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY, IMG_INIT_PNG, false) {
            for (std::size_t i = 0; i < this->tileset_textures.size(); i++) {
                grid.emplace_back();
                for (Uint16 j = 0; j < this->window.get_height() / this->cell_size; j++) {
                    grid[i].emplace_back();
                    for (Uint16 k = 0; k < this->window.get_width() / this->cell_size; k++) {
                        grid[i][j].emplace_back(-1);
                    }
                }
            }
        }
        ~autotiler_demo() {
            for (std::size_t i = 0; i < this->tileset_textures.size(); i++) {
                SDL_DestroyTexture(this->tileset_textures[i]);
            }
        }
};

int main(int argc, char* args[]) {
    autotiler_demo a;
    return a.run();
}
