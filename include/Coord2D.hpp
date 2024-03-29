#ifndef COORD2D
#define COORD2D

#include <cmath>
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

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (metric > 4 || metric < 0) ? COORD_RELATE_COMMON : metric;
            return output;
        }
        static Coord2D<ArithType> getReferencePoint() {return ReferencePoint;}
        static Coord2D<ArithType> setReferencePoint(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) {
            Coord2D<ArithType> output(ReferencePoint.getX(), ReferencePoint.getY());
            ReferencePoint = coord;
            return output;
        }
        static Coord2D<ArithType> setReferencePoint(const ArithType &x, const ArithType &y) {return setReferencePoint(Coord2D<ArithType>(x, y));}

        ArithType getX() const {return X;}
        ArithType setX(const ArithType &x) {
            ArithType output = X;
            X = x;
            return output;
        }
        ArithType moveX(const ArithType &amount) {
            ArithType output = X;
            X += amount;
            return output;
        }

        ArithType getY() const {return Y;}
        ArithType setY(const ArithType &y) {
            ArithType output = Y;
            Y = y;
            return output;
        }
        ArithType moveY(const ArithType &amount) {
            ArithType output = Y;
            Y += amount;
            return output;
        }

        Coord2D<ArithType> reflectX() {
            Coord2D<ArithType> output = *this;
            Y = -Y;
            return output;
        }
        Coord2D<ArithType> reflectY() {
            Coord2D<ArithType> output = *this;
            X = -X;
            return output;
        }
        // TODO: Add a reflection for any line
        Coord2D<ArithType> stretchX(const ArithType &scalar) {
            Coord2D<ArithType> output = *this;
            X *= scalar;
            return output;
        }
        Coord2D<ArithType> shrinkX(const ArithType &scalar) {
            Coord2D<ArithType> output = *this;
            X /= scalar;
            return output;
        }
        Coord2D<ArithType> stretchY(const ArithType &scalar) {
            Coord2D<ArithType> output = *this;
            Y *= scalar;
            return output;
        }
        Coord2D<ArithType> shrinkY(const ArithType &scalar) {
            Coord2D<ArithType> output = *this;
            Y /= scalar;
            return output;
        }
        // TODO: Add a stretch along any line

        Coord2D<ArithType> rotate(const Coord2D<ArithType> &pivot, double angle = M_PI_2) {
            Coord2D<ArithType> output = *this;
            double s = std::sin(angle);
            double c = std::cos(angle);
            X -= pivot.getX();
            Y -= pivot.getY();
            X = X * c - Y * s + pivot.getX();
            Y = X * s + Y * c + pivot.getY();
            return output;
        }
        Coord2D<ArithType> rotate(double angle = M_PI_2) {return rotate(ReferencePoint, angle);}

        void operator= (const Coord2D<ArithType> &coord) {
            setX(coord.getX());
            setY(coord.getY());
        }
        Coord2D<ArithType> operator! () const {return Coord2D<ArithType>(-X, -Y);}
        std::string toString() const {return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";}

        bool operator== (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
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
        bool operator!= (const Coord2D<ArithType> &coord) const {return !(*this == coord);}
        bool operator< (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
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
        bool operator> (const Coord2D<ArithType> &coord) const {return coord < *this;}
        bool operator<= (const Coord2D<ArithType> &coord) const {return !(*this > coord);}
        bool operator>= (const Coord2D<ArithType> &coord) const {return !(*this < coord);}

        bool equal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this == coord);
            setRelationMetric(original);
            return output;
        }
        bool notequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !equals(coord, metric);}
        bool less(const Coord2D<ArithType> &coord, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this < coord);
            setRelationMetric(original);
            return output;
        }
        bool greater(const Coord2D<ArithType> &coord, const unsigned char &metric) const {
            unsigned char original = setRelationMetric(metric);
            bool output = (*this > coord);
            setRelationMetric(original);
            return output;
        }
        bool lessequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !greater(coord, metric);}
        bool greaterequal(const Coord2D<ArithType> &coord, const unsigned char &metric) const {return !less(coord, metric);}

        Coord2D<ArithType>& operator+= (const Coord2D<ArithType> &coord) {
            X += coord.getX();
            Y += coord.getY();
            return *this;
        }
        Coord2D<ArithType>& operator-= (const Coord2D<ArithType> &coord) {
            X -= coord.getX();
            Y -= coord.getY();
            return *this;
        }
        Coord2D<ArithType>& operator*= (const ArithType &scalar) {
            X *= scalar;
            Y *= scalar;
            return *this;
        }
        Coord2D<ArithType>& operator/= (const ArithType &scalar) {
            X /= scalar;
            Y /= scalar;
            return *this;
        }
        Coord2D<ArithType>& operator%= (const ArithType &denom) {
            X = std::fmod(X, denom);
            Y = std::fmod(Y, denom);
            return *this;
        }
        Coord2D<ArithType> operator+ (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X + coord.getX(), Y + coord.getY());}
        Coord2D<ArithType> operator- (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X - coord.getX(), Y - coord.getY());}
        Coord2D<ArithType> operator* (const ArithType &scalar) const {return Coord2D<ArithType>(X * scalar, Y * scalar);}
        Coord2D<ArithType> operator/ (const ArithType &scalar) const {return Coord2D<ArithType>(X / scalar, Y / scalar);}
        Coord2D<ArithType> operator% (const ArithType &denom) const {return Coord2D<ArithType>(std::fmod(X, denom), std::fmod(Y, denom));}

        ArithType distEuclid(const Coord2D<ArithType> &coord) const {
            double distance = std::sqrt(std::pow(X - coord.getX(), 2) + std::pow(Y - coord.getY(), 2));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType distEuclid() const {return distEuclid(ReferencePoint);}
        ArithType distTaxi(const Coord2D<ArithType> &coord) const {return std::fabs(X - coord.getX()) + std::fabs(Y - coord.getY());}
        ArithType distTaxi() const {return distTaxi(ReferencePoint);}
};

template <typename ArithType> unsigned char Coord2D<ArithType>::RelationMetric = COORD_RELATE_COMMON;
template <typename ArithType> Coord2D<ArithType> Coord2D<ArithType>::ReferencePoint = Coord2D<ArithType>(0, 0);

#endif /* COORD2D */
