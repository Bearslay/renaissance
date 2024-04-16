#include "Colors.hpp"
#include <iostream>

int main(int argc, char** argv) {
    ColorRGB rgb = {255, 0, 0, 255};
    ColorHSV hsv = toHSV(rgb);
    std::cout << (int)hsv.h << " " << (int)hsv.s << " " << (int)hsv.v << " " << (int)hsv.a << "\n";
    std::cout << "\n";
    for (short i = 0; i < 360; i += 10) {
        rgb = toRGB({i, 255, 255, 255});
        std::cout << (int)rgb.r << " " << (int)rgb.g << " " << (int)rgb.b << "\n";
    }

}
