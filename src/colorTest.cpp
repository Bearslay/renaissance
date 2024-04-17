#include "Colors.hpp"
#include <iostream>
#include "Utilities.hpp"
#include "Colors_Accurate.hpp"

int main(int argc, char** argv) {
    // for (double i = -C_2PI * 1.5; i <= C_2PI * 1.5; i += 0.1) {
    //     std::cout << i << " - " << btils::normalizeRadian(i) << "\n";
    // }
    // std::cout << btils::normalize<int>(100, 51) << "\n";

    // Color c((unsigned char)255, 0, 0);
    // for (short i = 0; i < 360; i++) {
    //     std::cout << i << " - " << c.rgbString() << "\n";
    //     c.adjH(1);
    // }

    HSVa c   = {0, 1, 1};
    RGBa c1;
    HSLa c2;
    for (short i = 0; i < 360; i += 30) {
        c.h = i;
        c1 = hsv2rgb(c);
        c2 = hsv2hsl(c);
        std::cout << i << " - " << c1.r * 255 << ", " << c1.g * 255 << ", " << c1.b * 255 << "\n";
        std::cout << i << " - " << c2.h << ", " << c2.s * 100 << ", " << c2.l * 100 << "\n";
    }
    std::cout << rgb2hsv({0, 255, 0}).h << "\n";

    return 0;
}
