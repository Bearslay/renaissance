#ifndef LINE2D
#define LINE2D

#include <cmath>
#include <string>

#include "Coord2D.hpp"

template <typename ArithType> class Line2D {
    private:
        Coord2D<ArithType> Coord = Coord2D<ArithType>(0, 0);
        double Slope = 0;
        bool Vertical = false;

    public:
        Line2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Line2D(const ArithType &x, const ArithType &y, const double &slope) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            Coord.setX(x);
            Coord.setY(y);

            if (std::isnan(slope)) {Vertical = true;}
            Slope = slope;
        }
        Line2D(const Coord2D<ArithType> &coord, const double &slope) {Line2D(coord.getX(), coord.getY(), slope);}
        Line2D(const Line2D<ArithType> &line) {
            Coord = line.Coord;
            Slope = line.Slope;
            Vertical = line.Vertical;
        }

        ArithType getX() const {return Coord.getX();}
        ArithType getY() const {return Coord.getY();}
        double getM() const {return Slope;}
        Line2D<ArithType> setX(const ArithType &x) {
            Line2D<ArithType> output(Coord.getX(), Coord.getY(), Slope);
            Coord.setX(x);
            return output;
        }
        Line2D<ArithType> setY(const ArithType &y) {
            Line2D<ArithType> output(Coord.getX(), Coord.getY(), Slope);
            Coord.setY(y);
            return output;
        }
        Line2D<ArithType> setM(const double &slope) {
            Line2D<ArithType> output(Coord.getX(), Coord.getY(), Slope);
            Slope = slope;
            if (std::isnan(slope)) {Vertical = true;}
            else {Vertical = false;}
            return output;
        }
        std::string toString() {return "[" + Coord.toString() + ", " + (Vertical ? "Undefined" : std::to_string(Slope)) + "]"}

        void operator = (const Line2D<ArithType> &line) {
            setX(line.getX());
            setY(line.getY());
            setM(line.getM());
        }
};

#endif /* LINE2D */
