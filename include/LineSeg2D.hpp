#ifndef LINESEG
#define LINESEG

#include "Coord2D.hpp"

template <typename ArithType> class LineSeg2D {
    private:
        Coord2D<ArithType> Position1 = Coord2D<ArithType>(0, 0);
        Coord2D<ArithType> Position2 = Coord2D<ArithType>(0, 0);

    public:
        LineSeg2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        LineSeg2D(Coord2D<ArithType> p1, Coord2D<ArithType> p2) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            Position1 = p1;
            Position2 = p2;
        }
        LineSeg2D(ArithType x1, ArithType y1, ArithType x2, ArithType y2) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            Position1 = Coord2D<ArithType>(x1, y1);
            Position2 = Coord2D<ArithType>(x2, y2);
        }

        Coord2D<ArithType> getPos1() const {return Position1;}
        Coord2D<ArithType> getPos2() const {return Position2;}
        ArithType getPos1X() const {return Position1.getX();}
        ArithType getPos1Y() const {return Position1.getY();}
        ArithType getPos2X() const {return Position2.getX();}
        ArithType getPos2Y() const {return Position2.getY();}
};

#endif /* LINESEG*/
