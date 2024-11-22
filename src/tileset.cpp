#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "btils.hpp"
#include "bengine.hpp"

class autotiler_demo : public bengine::loop {
    private:
        bengine::normalMouseState mstate;
        SDL_Point mouse_pos_grid = {0, 0};
        SDL_Point prev_mouse_pos_grid = {0, 0};

        unsigned char tileset_number = 0;
        std::vector<SDL_Texture*> tileset_textures = {
            window.loadTexture("dev/png/imperialPath/sheet4bit.png"),
            window.loadTexture("dev/png/imperialPath/sheet8bit.png"),
            window.loadTexture("dev/png/ironFence/sheet4bit.png"),
            window.loadTexture("dev/png/ironFence/sheet8bit.png")
        };

        std::vector<std::vector<std::vector<char>>> grid;
        Uint16 cell_size = 40;

        bengine::autotiler tiler;

        void handleEvent() override {
            switch (this->event.type) {
                case SDL_MOUSEMOTION:
                    this->mstate.updateMotion(this->event);
                    this->mouse_pos_grid = {this->mstate.posx() / this->cell_size, this->mstate.posy() / this->cell_size};
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    this->mstate.pressButton(this->event);
                    if (this->mstate.pressed(bengine::normalMouseState::button_names::LEFT_MOUSE_BUTTON) || this->mstate.pressed(bengine::normalMouseState::button_names::RIGHT_MOUSE_BUTTON)) {
                        if (this->tileset_number % 2 == 0) {
                            this->tiler.fourBit(grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->event.button.button == SDL_BUTTON_LEFT, false);
                        } else {
                            this->tiler.eightBit(grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->event.button.button == SDL_BUTTON_LEFT, false);
                        }
                        this->visualsChanged = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    this->mstate.releaseButton(this->event);
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
                            this->visualsChanged = true;
                        }
                    }
                    break;
            }
        }
        void compute() override {
            if (this->mouse_pos_grid.x != this->prev_mouse_pos_grid.x || this->mouse_pos_grid.y != this->prev_mouse_pos_grid.y) {
                this->prev_mouse_pos_grid = this->mouse_pos_grid;

                if (this->mstate.pressed(bengine::normalMouseState::button_names::LEFT_MOUSE_BUTTON) || this->mstate.pressed(bengine::normalMouseState::button_names::RIGHT_MOUSE_BUTTON)) {
                    if (this->tileset_number % 2 == 0) {
                        this->tiler.fourBit(this->grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->mstate.pressed(bengine::normalMouseState::button_names::LEFT_MOUSE_BUTTON), false);
                    } else {
                        this->tiler.eightBit(this->grid[this->tileset_number], this->mouse_pos_grid.x, this->mouse_pos_grid.y, this->mstate.pressed(bengine::normalMouseState::button_names::LEFT_MOUSE_BUTTON), false);
                    }
                    this->visualsChanged = true;
                }
            }

            this->mstate.stopMotion();
        }
        void render() override {
            // Background stuff
            this->window.fillRectangle(0, 0, this->window.getWidth(), this->window.getHeight(), bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::WHITE)]);
            for (std::size_t i = 0; i < this->grid.at(0).size(); i++) {
                for (std::size_t j = 0; j < this->grid.at(0).at(i).size(); j++) {
                    this->window.drawRectangle(j * this->cell_size, i * this->cell_size, this->cell_size, this->cell_size, bengine::window::presetColors[static_cast<Uint8>(bengine::window::presetColor::BLACK)]);
                }
            }

            for (std::size_t i = 0; i < this->tileset_textures.size(); i++) {
                for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                    for (std::size_t k = 0; k < grid.at(i).at(j).size(); k++) {
                        if (grid.at(i).at(j).at(k) >= 0) {
                            this->window.renderSDLTexture(this->tileset_textures.at(i), {this->grid.at(i).at(j).at(k) % (i % 2 == 0 ? 4 : 8) * 16, this->grid.at(i).at(j).at(k) / (i % 2 == 0 ? 4 : 8) * 16, 16, 16}, {(int)(k * this->cell_size), (int)(j * this->cell_size), this->cell_size, this->cell_size});
                        }
                    }
                }
            }
        }

    public:
        autotiler_demo() : bengine::loop("Autotiler Demo", 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY, IMG_INIT_PNG, false) {
            for (std::size_t i = 0; i < this->tileset_textures.size(); i++) {
                grid.emplace_back();
                for (Uint16 j = 0; j < this->window.getHeight() / this->cell_size; j++) {
                    grid[i].emplace_back();
                    for (Uint16 k = 0; k < this->window.getWidth() / this->cell_size; k++) {
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