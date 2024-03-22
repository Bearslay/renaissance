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
        ArithType X = 0;
        ArithType Y = 0;
        unsigned char RelationMetric = COORD_RELATE_COMMON;

    public:
        Coord2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord2D(const ArithType &x, const ArithType &y) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
        }
        Coord2D(const Coord2D<ArithType> &coord) {
            X = coord.getX();
            Y = coord.getY();
            RelationMetric = coord.getRelationMetric();
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        unsigned char getRelationMetric() const {return RelationMetric;}
        Coord2D<ArithType> setX(const ArithType &x) {
            Coord2D<ArithType> output(X, Y);
            X = x;
            return output;
        }
        Coord2D<ArithType> setY(const ArithType &y) {
            Coord2D<ArithType> output(X, Y);
            Y = y;
            return output;
        }
        unsigned char setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            if (metric > 4 || metric < 0) {RelationMetric = (5 + metric % 5) % 5;}
            return output;
        }
        std::string toString() {return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";}

        void operator = (const Coord2D<ArithType> &coord) {
            setX(coord.getX());
            setY(coord.getY());
            setRelationMetric(coord.RelationMetric);
        }
        Coord2D<ArithType> operator ! () const {return Coord2D<ArithType>(-X, -Y);}
        bool operator == (const Coord2D<ArithType> &coord) const {
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
        bool operator != (const Coord2D<ArithType> &coord) const {return !(Coord2D<ArithType>(X, Y) == coord);}
        bool operator < (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() < coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() < coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X < coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y < coord.getY();
            }
        }
        bool operator <= (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() <= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() <= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X <= coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y <= coord.getY();
            }
        }
        bool operator > (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() > coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() > coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X > coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y > coord.getY();
            }
        }
        bool operator >= (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() >= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() >= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X >= coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y >= coord.getY();
            }
        }

        Coord2D<ArithType> operator += (const Coord2D<ArithType> &coord) {
            Coord2D<ArithType> output(X, Y);
            X += coord.getX();
            Y += coord.getY();
            return output;
        }
        Coord2D<ArithType> operator -= (const Coord2D<ArithType> &coord) {
            Coord2D<ArithType> output(X, Y);
            X -= coord.getX();
            Y -= coord.getY();
            return output;
        }
        Coord2D<ArithType> operator *= (const ArithType &scalar) {
            Coord2D<ArithType> output(X, Y);
            X *= scalar;
            Y *= scalar;
            return output;
        }
        Coord2D<ArithType> operator + (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X + coord.getX(), Y + coord.getY());}
        Coord2D<ArithType> operator - (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X - coord.getX(), Y - coord.getY());}
        Coord2D<ArithType> operator * (const ArithType &scalar) const {return Coord2D<ArithType>(X * scalar, Y * scalar);}

        ArithType distEuclid(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) const {
            double distance = std::sqrt(std::pow(X - coord.getX(), 2) + std::pow(Y - coord.getY(), 2));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType distTaxi(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) const {return std::fabs(X - coord.getX()) + std::fabs(Y - coord.getY());}
};

#endif /* COORD2D */
