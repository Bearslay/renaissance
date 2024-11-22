#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "btils.hpp"
#include "bengine.hpp"

class physics_sim : public bengine::loop {
    private:

        void handleEvent() override {
            
        }
        void compute() override {

        }
        void render() override {

        }

    public:
        physics_sim() : bengine::loop("Physics Simulation", 1280, 720, SDL_WINDOW_SHOWN, IMG_INIT_PNG, true) {}
};

int main(int argc, char* args[]) {
    physics_sim p;
    p.run();
}
