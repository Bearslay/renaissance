#include "Colors.hpp"
#include <iostream>
#include "Utilities.hpp"

int main(int argc, char** argv) {
    for (double i = -C_2PI * 1.5; i <= C_2PI * 1.5; i += 0.1) {
        std::cout << i << " - " << btils::normalizeRadian(i) << "\n";
    }
    std::cout << btils::normalize<int>(100, 51) << "\n";

    return 0;
}
