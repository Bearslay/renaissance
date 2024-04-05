#ifndef COORD2D
#define COORD2D

#include <cmath>
#include <vector>
#include <string>

#ifndef COORD2D_RELATIONS
#define COORD2D_RELATIONS
#define COORD_RELATE_COMMON 0
#define COORD_RELATE_EUCLID 1
#define COORD_RELATE_TAXICAB 2
#define COORD_RELATE_XCOORD 3
#define COORD_RELATE_YCOORD 4
#endif /* COORD2D_RELATIONS */

template <typename ArithType> class Coord2D {
    private:
        ArithType X;
        ArithType Y;

        static unsigned char RelationMetric;
        static Coord2D<ArithType> ReferencePoint;

    public:
        Coord2D() : X(0), Y(0) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord2D(const ArithType &x, const ArithType &y) : X(x), Y(y) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord2D(const Coord2D<ArithType> &coord) : X(coord.getX()), Y(coord.getY()) {}

        static unsigned char      getRelationMetric() {return RelationMetric;}
        static unsigned char      setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            const unsigned char output = RelationMetric;
            RelationMetric = (metric > 4 || metric < 0) ? COORD_RELATE_COMMON : metric;
            return output;
        }
        static Coord2D<ArithType> getReferencePoint() {return ReferencePoint;}
        static Coord2D<ArithType> setReferencePoint(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) {
            const Coord2D<ArithType> output(ReferencePoint.getX(), ReferencePoint.getY());
            ReferencePoint = coord;
            return output;
        }
        static Coord2D<ArithType> setReferencePoint(const ArithType &x, const ArithType &y) {return setReferencePoint(Coord2D<ArithType>(x, y));}

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType setX(const ArithType &x) {
            const ArithType output = X;
            X = x;
            return output;
        }
        ArithType adjX(const ArithType &amount) {
            const ArithType output = X;
            X += amount;
            return output;
        }
        ArithType setY(const ArithType &y) {
            const ArithType output = Y;
            Y = y;
            return output;
        }
        ArithType adjY(const ArithType &amount) {
            const ArithType output = Y;
            Y += amount;
            return output;
        }

        Coord2D<ArithType> reflectX() {
            const Coord2D<ArithType> output = *this;
            Y = -Y;
            return output;
        }
        Coord2D<ArithType> reflectY() {
            const Coord2D<ArithType> output = *this;
            X = -X;
            return output;
        }
        // TODO: Add a reflection for any line
        Coord2D<ArithType> stretchX(const ArithType &scalar) {
            const Coord2D<ArithType> output = *this;
            X *= scalar;
            return output;
        }
        Coord2D<ArithType>  shrinkX(const ArithType &scalar) {
            const Coord2D<ArithType> output = *this;
            X /= scalar;
            return output;
        }
        Coord2D<ArithType> stretchY(const ArithType &scalar) {
            const Coord2D<ArithType> output = *this;
            Y *= scalar;
            return output;
        }
        Coord2D<ArithType>  shrinkY(const ArithType &scalar) {
            const Coord2D<ArithType> output = *this;
            Y /= scalar;
            return output;
        }
        // TODO: Add a stretch along any line
        Coord2D<ArithType>   rotate(const Coord2D<ArithType> &pivot, const double &angle) {
            const Coord2D<ArithType> output = *this;
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            X = (X - pivot.getX()) * c - (Y - pivot.getY()) * s + pivot.getX();
            Y = (X - pivot.getX()) * s + (Y - pivot.getY()) * c + pivot.getY();
            return output;
        }
        Coord2D<ArithType>   rotate(const double &angle) {return rotate(ReferencePoint, angle);}

        static std::vector<Coord2D<ArithType>> rotatePoints(const std::vector<Coord2D<ArithType>> &points, const Coord2D<ArithType> &pivot, const double &angle) {
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            std::vector<Coord2D<ArithType>> output;
            Coord2D<ArithType> point;
            for (unsigned long i = 0; i < points.size(); i++) {
                point = points.at(i);
                point.setX((point.getX() - pivot.getX()) * c - (point.getY() - pivot.getY()) * s + pivot.getX());
                point.setX((point.getX() - pivot.getX()) * s + (point.getY() - pivot.getY()) * c + pivot.getY());
                output.emplace_back(point);
            }
            return output;
        }
        static std::vector<Coord2D<ArithType>> rotatePoints(const std::vector<Coord2D<ArithType>> &points, const double &angle) {return rotatePoints(points, Coord2D<ArithType>(0, 0), angle);}

        void operator=(const Coord2D<ArithType> &coord) {
            setX(coord.getX());
            setY(coord.getY());
        }
        Coord2D<ArithType>     operator!() const {return Coord2D<ArithType>(-X, -Y);}
        std::string             toString() const {return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";}
        std::vector<ArithType>   toVector() const {return {X, Y};}

        bool        equal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {
            switch (metric) {
                default:
                case COORD_RELATE_COMMON:
                    return X == coord.getX() && Y == coord.getY();
                case COORD_RELATE_EUCLID:
                    return distEuclid() == coord.distEuclid();
                case COORD_RELATE_TAXICAB:
                    return distTaxi() == coord.distTaxi();
                case COORD_RELATE_XCOORD:
                    return X == coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y == coord.getY();
            }
        }
        bool     notequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !equal(coord, metric);}
        bool         less(const Coord2D<ArithType> &coord, const unsigned char &metric) const {
            switch (metric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return distTaxi() < coord.distTaxi();
                case COORD_RELATE_EUCLID:
                    return distEuclid() < coord.distEuclid();
                case COORD_RELATE_XCOORD:
                    return X < coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y < coord.getY();
            }
        }
        bool      greater(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return coord.less(*this, RelationMetric);}
        bool    lessequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !greater(coord, metric);}
        bool greaterequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !less(coord, metric);}
        bool   operator==(const Coord2D<ArithType> &coord) const {return        equal(coord, RelationMetric);}
        bool   operator!=(const Coord2D<ArithType> &coord) const {return     notequal(coord, RelationMetric);}
        bool    operator<(const Coord2D<ArithType> &coord) const {return         less(coord, RelationMetric);}
        bool    operator>(const Coord2D<ArithType> &coord) const {return      greater(coord, RelationMetric);}
        bool   operator<=(const Coord2D<ArithType> &coord) const {return    lessequal(coord, RelationMetric);}
        bool   operator>=(const Coord2D<ArithType> &coord) const {return greaterequal(coord, RelationMetric);}

        Coord2D<ArithType>& operator+=(const Coord2D<ArithType> &coord) {
            X += coord.getX();
            Y += coord.getY();
            return *this;
        }
        Coord2D<ArithType>& operator-=(const Coord2D<ArithType> &coord) {
            X -= coord.getX();
            Y -= coord.getY();
            return *this;
        }
        Coord2D<ArithType>& operator*=(const ArithType &scalar) {
            X *= scalar;
            Y *= scalar;
            return *this;
        }
        Coord2D<ArithType>& operator/=(const ArithType &scalar) {
            X /= scalar;
            Y /= scalar;
            return *this;
        }
        Coord2D<ArithType>& operator%=(const ArithType &denom)  {
            X = std::fmod(X, denom);
            Y = std::fmod(Y, denom);
            return *this;
        }
        Coord2D<ArithType>   operator+(const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X + coord.getX(), Y + coord.getY());}
        Coord2D<ArithType>   operator-(const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X - coord.getX(), Y - coord.getY());}
        Coord2D<ArithType>   operator*(const ArithType &scalar)         const {return Coord2D<ArithType>(X * scalar, Y * scalar);}
        Coord2D<ArithType>   operator/(const ArithType &scalar)         const {return Coord2D<ArithType>(X / scalar, Y / scalar);}
        Coord2D<ArithType>   operator%(const ArithType &denom)          const {return Coord2D<ArithType>(std::fmod(X, denom), std::fmod(Y, denom));}

        ArithType distEuclid(const Coord2D<ArithType> &coord) const {
            const double distance = std::sqrt((X - coord.getX()) * (X - coord.getX()) + (Y - coord.getY()) * (Y - coord.getY()));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType distEuclid()                                const {return distEuclid(ReferencePoint);}
        ArithType   distTaxi(const Coord2D<ArithType> &coord) const {return std::fabs(X - coord.getX()) + std::fabs(Y - coord.getY());}
        ArithType   distTaxi()                                const {return distTaxi(ReferencePoint);}
};

template <typename ArithType> unsigned char Coord2D<ArithType>::RelationMetric = COORD_RELATE_COMMON;
template <typename ArithType> Coord2D<ArithType> Coord2D<ArithType>::ReferencePoint = Coord2D<ArithType>(0, 0);

#endif /* COORD2D */
