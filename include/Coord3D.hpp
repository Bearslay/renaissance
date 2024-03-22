#ifndef COORD3D
#define COORD3D

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

#ifndef COORD3D_RELATIONS
#define COORD3D_RELATIONS
#define COORD_RELATE_ZCOORD 5
#endif /* COORD3D_RELATIONS */

template <typename ArithType> class Coord3D {
    private:
        ArithType X = 0;
        ArithType Y = 0;
        ArithType Z = 0;
        unsigned char RelationMetric = COORD_RELATE_COMMON;

    public:
        Coord3D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord3D(const ArithType &x, const ArithType &y, const ArithType &z) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
            Z = z;
        }
        Coord3D(const Coord3D<ArithType> &coord) {
            X = coord.X;
            Y = coord.Y;
            Z = coord.Z;
            RelationMetric = coord.RelationMetric;
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType getZ() const {return Z;}
        unsigned char getRelationMetric() const {return RelationMetric;}
        Coord3D<ArithType> setX(const ArithType &x) {
            Coord3D<ArithType> output(X, Y, Z);
            X = x;
            return output;
        }
        Coord3D<ArithType> setY(const ArithType &y) {
            Coord3D<ArithType> output(X, Y, Z);
            Y = y;
            return output;
        }
        Coord3D<ArithType> setY(const ArithType &z) {
            Coord3D<ArithType> output(X, Y, Z);
            Z = z;
            return output;
        }
        unsigned char setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            if (metric > 5 || metric < 0) {RelationMetric = (6 + metric % 6) % 6;}
            return output;
        }
        std::string toString() {return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";}

        void operator = (const Coord3D<ArithType> &coord) {
            X = coord.X;
            Y = coord.Y;
            Z = coord.Z;
            RelationMetric = coord.RelationMetric;
        }
        Coord3D<ArithType> operator ! () const {return Coord3D<ArithType>(-X, -Y, -Z);}
        bool operator == (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                    return X == coord.X && Y == coord.Y && Z == coord.Z;
                case COORD_RELATE_EUCLID:
                    return distEuclid() == coord.distEuclid();
                case COORD_RELATE_TAXICAB:
                    return distTaxi() == coord.distTaxi();
                case COORD_RELATE_XCOORD:
                    return X == coord.X;
                case COORD_RELATE_YCOORD:
                    return Y == coord.Y;
                case COORD_RELATE_ZCOORD:
                    return Z == coord.Z;
            }
        }
        bool operator != (const Coord3D<ArithType> &coord) const {return !(Coord3D<ArithType>(X, Y, Z) == coord);}
        bool operator < (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() < coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() < coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X < coord.X;
                case COORD_RELATE_YCOORD:
                    return Y < coord.Y;
                case COORD_RELATE_ZCOORD:
                    return Z < coord.Z;
            }
        }
        bool operator <= (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() <= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() <= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X <= coord.X;
                case COORD_RELATE_YCOORD:
                    return Y <= coord.Y;
                case COORD_RELATE_ZCOORD:
                    return Z <= coord.Z;
            }
        }
        bool operator > (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() > coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() > coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X > coord.X;
                case COORD_RELATE_YCOORD:
                    return Y > coord.Y;
                case COORD_RELATE_ZCOORD:
                    return Z > coord.Z;
            }
        }
        bool operator >= (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICAB:
                    return taxicabDistance() >= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() >= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X >= coord.X;
                case COORD_RELATE_YCOORD:
                    return Y >= coord.Y;
                case COORD_RELATE_ZCOORD:
                    return Z >= coord.Z;
            }
        }

        Coord3D<ArithType> operator += (const Coord3D<ArithType> &coord) {
            Coord3D<ArithType> output(X, Y, Z);
            X += coord.X;
            Y += coord.Y;
            Z += coord.Z;
            return output;
        }
        Coord3D<ArithType> operator -= (const Coord3D<ArithType> &coord) {
            Coord3D<ArithType> output(X, Y, Z);
            X -= coord.X;
            Y -= coord.Y;
            Z -= coord.Z;
            return output;
        }
        Coord3D<ArithType> operator *= (const ArithType &scalar) {
            Coord3D<ArithType> output(X, Y, Z);
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            return output;
        }
        Coord3D<ArithType> operator + (const Coord3D<ArithType> &coord) const {return Coord3D<ArithType>(X + coord.X, Y + coord.Y, Z + coord.Z);}
        Coord3D<ArithType> operator - (const Coord3D<ArithType> &coord) const {return Coord3D<ArithType>(X - coord.X, Y - coord.Y, Z - coord.Z);}
        Coord3D<ArithType> operator * (const ArithType &scalar) const {return Coord3D<ArithType>(X * scalar, Y * scalar, Z * scalar);}

        ArithType distEuclid(const Coord3D<ArithType> &coord = Coord3D<ArithType>(0, 0, 0)) const {
            double distance = std::sqrt(std::pow(X - coord.X, 2) + std::pow(Y - coord.Y, 2) + std::pow(Z - coord.Z, 2));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType distTaxi(const Coord3D<ArithType> &coord = Coord3D<ArithType>(0, 0, 0)) const {return std::fabs(X - coord.X) + std::fabs(Y - coord.Y) + std::fabs(Z - coord.Z);}
};

#endif /* COORD3D */
