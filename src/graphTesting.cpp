#include <iostream>
#include "Coord2D.hpp"

int main() {
    Coord2D<int> c1(3, 4);
    Coord2D<int> c2(0, 0);
    c2 += c1 + Coord2D<int>(4, -3);

    for (unsigned char i = 0; i < 3; i++) {
        std::cout << c1.toString() << "  " << c2.toString() << "\n";
        std::cout << "Euclid:  " << c1.distEuclid() << "  " << c2.distEuclid() << "\n";
        std::cout << "Taxicab: " << c1.distTaxi() << "  " << c2.distTaxi() << "\n";
        std::cout << "==: " << (c1 == c2) << "\n";
        std::cout << "!=: " << (c1 != c2) << "\n";
        std::cout << "<:  " << (c1 < c2) << "\n";
        std::cout << ">:  " << (c1 > c2) << "\n";
        std::cout << "<=: " << (c1 <= c2) << "\n";
        std::cout << ">=: " << (c1 >= c2) << "\n\n";

        switch (i) {
            case 0:
                c1.setRelationMetric(COORD_RELATE_TAXICAB);
                c1.setReferencePoint(1, 0);
                c2.adjY(-1);
                break;
            case 1:
                c1 *= 3;
                c2 *= 2;
                break;
        }
    }

    return 0;
}
