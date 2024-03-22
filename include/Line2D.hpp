#ifndef LINE2D
#define LINE2D

#include <cmath>
#include <string>

#include "Coord2D.hpp"

#ifndef LINE2D_RELATIONS
#define LINE2D_RELATIONS
#define LINE2D_RELATE_COMMON 0
#define LINE2D_RELATE_SLOPE 1
#define LINE2D_RELATE_XINT 2
#define LINE2D_RELATE_YINT 3
#endif /* LINE2D_RELATIONS */

#ifndef LINE2D_STRINGS
#define LINE2D_STRINGS
#define LINE2D_STRING_POINTSLOPE 0
#define LINE2D_STRING_STANDARD 1
#define LINE2D_STRING_YINT 2
#define LINE2D_STRING_XINT 3
#endif /* LINE2D_STRINGS */

template <typename ArithType> class Line2D {
    private:
        Coord2D<ArithType> Point = Coord2D<ArithType>(0, 0);
        double Slope = 1;
        ArithType YInt = 0;
        ArithType XInt = 0;
        unsigned char RelationMetric = LINE2D_RELATE_COMMON;

        void calcIntercepts() {
            if (Slope == NULL) {
                YInt = NULL;
                XInt = Point.getX();
                return;
            }
            if (Slope == 0) {
                YInt = Point.getY();
                XInt = NULL;
                return;
            }

            if (std::is_integral<ArithType>::value) {
                YInt = std::round(-Slope * Point.getX() + Point.getY());
                XInt = std::round((-Point.getY() + Slope * Point.getX()) / Slope);
            } else {
                YInt = -Slope * Point.getX() + Point.getY();
                XInt = (-Point.getY() + Slope * Point.getX()) / Slope;
            }
        }

    public:
        Line2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Line2D(const Coord2D<ArithType> &point, const double &slope) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            Point = point;
            Slope = std::isnan(slope) ? NULL : slope;
            calcIntercepts();
        }
        Line2D(const Line2D<ArithType> &line) {
            Point = Coord2D<ArithType>(line.getPX(), line.getPY());
            Slope = line.getSlope();
            YInt = line.getYInt();
            XInt = line.getXInt();
            RelationMetric = line.getRelationMetric();
        }

        ArithType getPX() const {return Point.getX();}
        ArithType getPY() const {return Point.getY();}
        double getSlope() const {return Slope;}
        ArithType getYInt() const {return YInt;}
        ArithType getXInt() const {return XInt;}
        unsigned char getRelationMetric() const {return RelationMetric;}

        ArithType calcY(const ArithType &x) const {
            if (Slope == NULL) {return NULL;}
            if (Slope == 0) {return Point.getY();}

            if (std::is_integral<ArithType>::value) {return std::round(Slope * x - Slope * Point.getX() + Point.getY());}
            return Slope * x - Slope * Point.getX() + Point.getY();
        }
        ArithType calcX(const ArithType &y) const {
            if (Slope == NULL) {return Point.getX();}
            if (Slope == 0) {return NULL;}

            if (std::is_integral<ArithType>::value) {return std::round((y - Point.getY() + Slope * Point.getX()) / Slope);}
            return (y - Point.getY() + Slope * Point.getX()) / Slope;
        }

        double setSlope(const double &slope) {
            double output = Slope;
            Slope = std::isnan(slope) ? NULL : slope;
            calcIntercepts();
            return output;
        }

        unsigned char setRelationMetric(const unsigned char &metric = LINE2D_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            if (metric > 3 || metric < 0) {RelationMetric = (3 + metric % 3) % 3;}
            return output;
        }
        std::string toString(unsigned char form = LINE2D_STRING_POINTSLOPE) {
            if (Slope == NULL) {return "x = " + std::to_string(Point.getX());}
            if (Slope == 0) {return "y = " + std::to_string(Point.getY());}

            switch (form) {
                default:
                case LINE2D_STRING_POINTSLOPE:
                    return "y" + (Point.getY() == 0 ? "" : ((Point.getY() < 0 ? " + " : " - ") + std::to_string(std::fabs(Point.getY())))) + " = " + std::to_string(Slope) + (Point.getX() == 0 ? "x" : ((Point.getX() < 0 ? "(x + " : "(x - ") + std::to_string(std::fabs(Point.getX())) + ")"));
                case LINE2D_STRING_STANDARD:
                    return std::to_string(-Slope) + "x + y = " + std::to_string(Point.getY() - Slope * Point.getX());
                case LINE2D_RELATE_YINT:
                    return "y = " + std::to_string(Slope) + "x" + (YInt == 0 ? "" : ((YInt < 0 ? " - " : " + ") + std::to_string(std::fabs(YInt))));
                case LINE2D_RELATE_XINT:
                    return "x = " + std::to_string(1 / Slope) + "y" + (XInt == 0 ? "" : ((XInt < 0 ? " - " : " + ") + std::to_string(std::fabs(XInt))));
            }
        }
};

#endif /* LINE2D */
