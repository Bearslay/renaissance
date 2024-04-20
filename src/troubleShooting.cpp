#include <iostream>

#include "Utilities.hpp"


int main(int argc, char** argv) {
    btils::UnitInterval a(0.25);
    std::cout << a.get() << "\n";

    // int b = 10;
    std::cout << (int)(a += 239) << "\n";
    std::cout << (a < 0.4) << "\n";
    a++;
    std::cout << (double)a << "\n";

    btils::UnitInterval b(1.0);
    std::cout << (double)b << "\n";
}
