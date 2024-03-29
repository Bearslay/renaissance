#ifndef LINE2D
#define LINE2D

#include <cmath>
#include <string>

#include "Coord2D.hpp"

#ifndef LINE2D_RELATIONS
#define LINE2D_RELATIONS
#define LINE2D_RELATE_COMMON 0
#define LINE2D_RELATE_SLOPE 1
#define LINE2D_RELATE_X 2
#define LINE2D_RELATE_Y 3
#define LINE2D_RELATE_XINT 4
#define LINE2D_RELATE_YINT 5
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
        Coord2D<ArithType> Point;
        double Slope;
        ArithType YInt;
        ArithType XInt;

        static unsigned char RelationMetric;
        static Coord2D<ArithType> ReferencePoint;

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
        Line2D() : Point(Coord2D<ArithType>(0, 0)), Slope(1), YInt(0), XInt(0)  {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Line2D(const Coord2D<ArithType> &point, const double &slope) : Point(point), Slope(std::isnan(slope) ? NULL : slope) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            calcIntercepts();
        }
        Line2D(const Line2D<ArithType> &line) : Point(Coord2D<ArithType>(line.getPX(), line.getPY())), Slope(line.getSlope()), YInt(line.getYInt()), XInt(line.getXInt()) {}

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (metric > 5 || metric < 0) ? COORD_RELATE_COMMON : metric;
            return output;
        }
        static Coord2D<ArithType> getReferencePoint() {return ReferencePoint;}
        static Coord2D<ArithType> setReferencePoint(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) {
            Coord2D<ArithType> output(ReferencePoint.getX(), ReferencePoint.getY());
            ReferencePoint = coord;
            return output;
        }
        static Coord2D<ArithType> setReferencePoint(const ArithType &x, const ArithType &y) {return setReferencePoint(Coord2D<ArithType>(x, y));}

        Coord2D<ArithType> getPoint() const {return Point;}

        ArithType getX() const {return Point.getX();}
        ArithType setX(const ArithType &x) {
            ArithType output = Point.getX();
            Point.setX(x);
            calcIntercepts();
            return output;
        }
        ArithType moveX(const ArithType &amount) {
            ArithType output = Point.getX();
            Point.moveX(amount);
            calcIntercepts();
            return output;
        }

        ArithType getY() const {return Point.getY();}
        ArithType setY(const ArithType &y) {
            ArithType output = Point.getY();
            Point.setY(y);
            calcIntercepts();
            return output;
        }
        ArithType moveY(const ArithType &amount) {
            ArithType output = Point.getY();
            Point.moveY(amount);
            calcIntercepts();
            return output;
        }

        ArithType getXInt() const {return XInt;}
        ArithType setXInt(const ArithType &x) {
            ArithType output = XInt;
            XInt = x;
            calcIntercepts();
            return output;
        }
        ArithType moveXInt(const ArithType &amount) {
            ArithType output = XInt;
            XInt += amount;
            calcIntercepts();
            return output;
        }

        ArithType getYInt() const {return YInt;}
        ArithType setYInt(const ArithType &y) {
            ArithType output = YInt;
            YInt = y;
            calcIntercepts();
            return output;
        }
        ArithType moveYInt(const ArithType &amount) {
            ArithType output = YInt;
            YInt += amount;
            calcIntercepts();
            return output;
        }

        double getSlope() const {return Slope;}
        double setSlope(const double &slope) {
            double output = Slope;
            Slope = slope;
            calcIntercepts();
            return output;
        }
        double pointAngle(const double &angle) {
            double output = Slope;
            while (angle < 0) {angle += 2 * M_PI;}
            while (angle > 2 * M_PI) {angle -= 2 * M_PI;}
            if (angle == M_PI_2 || angle == 3 * M_PI_2) {
                Slope = std::sqrt(-1);
                return output;
            }
            Slope = std::sin(angle) / std::cos(angle);
            calcIntercepts();
            return output;
        }

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

        void operator= (const Line2D<ArithType> &line) {
            setX(line.getX());
            setY(line.getY());
            setSlope(line.getSlope());
            setYInt(line.getYInt());
            setXInt(line.getXInt());
        }
        Line2D<ArithType> operator! () const {return Line2D<ArithType>(!Point, -1 / Slope);}
        std::string toString(unsigned char form = LINE2D_STRING_POINTSLOPE) const {
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

        bool operator== (const Line2D<ArithType> &line) const {
            switch (RelationMetric) {
                default:
                case LINE2D_RELATE_COMMON:
                    return Point.getX() == line.getX() && Point.getY() == line.getY() && Slope == line.getSlope();
                case LINE2D_RELATE_SLOPE:
                    return Slope == line.getSlope();
                case LINE2D_RELATE_X:
                    return Point.getX() == line.getX();
                case LINE2D_RELATE_Y:
                    return Point.getY() == line.getY();
                case LINE2D_RELATE_YINT:
                    return YInt == line.getYInt();
                case LINE2D_RELATE_XINT:
                    return XInt == line.getXInt();
            }
        }
        bool operator!= (const Line2D<ArithType> &line) const {return !(*this == line);}
        bool operator< (const Line2D<ArithType> &line) const {
            switch (RelationMetric) {
                case LINE2D_RELATE_COMMON:
                case LINE2D_RELATE_SLOPE:
                    return Slope < line.getSlope();
                case LINE2D_RELATE_X:
                    return Point.getX() < line.getX();
                case LINE2D_RELATE_Y:
                    return Point.getY() < line.getY();
                case LINE2D_RELATE_YINT:
                    return YInt < line.getYInt();
                case LINE2D_RELATE_XINT:
                    return XInt < line.getXInt();
            }
        }
        bool operator> (const Line2D<ArithType> &line) const {return line < *this;}
        bool operator<= (const Line2D<ArithType> &line) const {return !(*this > line);}
        bool operator>= (const Line2D<ArithType> &line) const {return !(*this < line);}

        bool equal(const Line2D<ArithType> &line, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this == line);
            setRelationMetric(original);
            return output;
        }
        bool notequal(const Line2D<ArithType> &line, const unsigned char &metric) const {return !equals(line, metric);}
        bool less(const Line2D<ArithType> &line, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this < line);
            setRelationMetric(original);
            return output;
        }
        bool greater(const Line2D<ArithType> &line, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this > line);
            setRelationMetric(original);
            return output;
        }
        bool lessequal(const Line2D<ArithType> &line, const unsigned char &metric) const {return !greater(line, metric);}
        bool greaterequal(const Line2D<ArithType> &line, const unsigned char &metric) const {return !less(line, metric);}

        Line2D<ArithType>& operator+= (const Line2D<ArithType> &line) {
            Slope += line.getSlope();
            Point = Coord2D<ArithType>((Point.getX() * Slope + line.getX() * line.getSlope()) / Slope, Point.getY() + line.getY());
            Point.shrinkX(Slope);
            Point.shrinkY(2);
            Slope /= 2;
            calcIntercepts();
            return *this;
        }
        Line2D<ArithType>& operator-= (const Line2D<ArithType> &line) {
            Slope -= line.getSlope();
            Point = Coord2D<ArithType>((Point.getX() * Slope - line.getX() * line.getSlope()) / Slope, Point.getY() - line.getY());
            Point.shrinkX(Slope);
            Slope = std::sqrt(-1);
            calcIntercepts();
            return *this;
        }
        Line2D<ArithType>& operator*= (const ArithType &scalar) {
            Slope *= scalar;
            Point.stretchY(scalar);
            calcIntercepts();
            return *this;
        }
        Line2D<ArithType>& operator/= (const ArithType &scalar) {
            Slope /= scalar;
            Point.shrinkY(scalar);
            calcIntercepts();
            return *this;
        }
        Line2D<ArithType>& operator%= (const ArithType &denom) {
            Slope = std::fmod(Slope, denom);
            Point.setY(std::fmod(Point.getY(), denom));
            calcIntercepts();
            return *this;
        }
        Line2D<ArithType> operator+ (const Line2D<ArithType> &line) const {
            double slope = Slope + line.getSlope();
            Line2D<ArithType> output(Coord2D<ArithType>((Point.getX() * slope - line.getX() * line.getSlope()) / slope, Point.getY() - line.getY()), slope);
            output.setX(output.getX() / Slope);
            output.setY(output.getY() * 2);
            output.setSlope(output.getSlope() / 2);
            return output;
        }
        Line2D<ArithType> operator- (const Line2D<ArithType> &line) const {
            double slope = Slope - line.getSlope();
            Line2D<ArithType> output(Coord2D<ArithType>((Point.getX() * slope - line.getX() * line.getSlope()) / slope, Point.getY() - line.getY()), slope);
            output.setX(output.getX() / Slope);
            output.setSlope(std::sqrt(-1));
            return output;
        }
        Line2D<ArithType> operator* (const ArithType &scalar) const {return Line2D<ArithType>(Coord2D<ArithType>(Point.getX(), Point.getY() * scalar), Slope * scalar);}
        Line2D<ArithType> operator/ (const ArithType &scalar) const {return Line2D<ArithType>(Coord2D<ArithType>(Point.getX(), Point.getY() / scalar), Slope / scalar);}
        Line2D<ArithType> operator% (const ArithType &denom) const {return Line2D<ArithType>(Coord2D<ArithType>(Point.getX(), std::fmod(Point.getY(), denom)), std::fmod(Slope, denom));}
};

template <typename ArithType> unsigned char Line2D<ArithType>::RelationMetric = COORD_RELATE_COMMON;
template <typename ArithType> Coord2D<ArithType> Line2D<ArithType>::ReferencePoint = Coord2D<ArithType>(0, 0);

#endif /* LINE2D */
