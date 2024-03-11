#ifndef MATHCOORD
#define MATHCOORD

#include <cmath>
#include <vector>
#include <string>

#include "astr.hpp"

#define COORD_RELATE_COMMON 0
#define COORD_RELATE_EUCLID 1
#define COORD_RELATE_TAXICB 2
#define COORD_RELATE_XCOORD 3
#define COORD_RELATE_YCOORD 4
#define COORD_RELATE_ZCOORD 5

template <typename ArithType> class Coord2D {
    private:
        ArithType X = 0;
        ArithType Y = 0;
        unsigned char RelationMetric = COORD_RELATE_COMMON;

    public:
        Coord2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord2D(ArithType x, ArithType y) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
        }
        Coord2D(const Coord2D<ArithType> &coord) {
            X = coord.X;
            Y = coord.Y;
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        Coord2D<ArithType> setX(ArithType x) {
            Coord2D<ArithType> output(X, Y);
            X = x;
            return output;
        }
        Coord2D<ArithType> setY(ArithType y) {
            Coord2D<ArithType> output(X, Y);
            Y = y;
            return output;
        }

        std::vector<ArithType> toVector() const {return {X, Y};}
        std::string toString(bool specifyPositive = false, short round = 3) const {return "(" + astr::toString(astr::round(X, round), specifyPositive) + ", " + astr::toString(astr::round(Y, round), specifyPositive) + ")";}
        std::string toString_Places(unsigned int beforeDecimal, unsigned int afterDecimal = 0, bool add = false, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Places(astr::round(X, round), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(Y, round), beforeDecimal, afterDecimal, add, specifyPositive) + ")";}
        std::string toString_Length(unsigned int length, bool leading = true, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Length(astr::round(X, round), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(Y, round), length, leading, specifyPositive) + ")";}
        std::string toString_Sci(short decimals = 2, short exponentDigits = 1, bool e = true, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Sci(X, decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(Y, decimals, exponentDigits, e, specifyPositive) + ")";}

        Coord2D<ArithType> operator ! () const {return Coord2D<ArithType>(-X, -Y);}
        bool operator == (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                    return X == coord.X && Y == coord.Y;
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() == coord.euclideanDistance();
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() == coord.taxicabDistance();
                case COORD_RELATE_XCOORD:
                    return X == coord.X;
                case COORD_RELATE_YCOORD:
                    return Y == coord.Y;
            }
        }
        bool operator != (const Coord2D<ArithType> &coord) const {return !(Coord2D<ArithType>(X, Y) == coord);}
        bool operator < (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() < coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() < coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X < coord.X;
                case COORD_RELATE_YCOORD:
                    return Y < coord.Y;
            }
        }
        bool operator <= (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() <= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() <= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X <= coord.X;
                case COORD_RELATE_YCOORD:
                    return Y <= coord.Y;
            }
        }
        bool operator > (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_TAXICB:
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() > coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() > coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X > coord.X;
                case COORD_RELATE_YCOORD:
                    return Y > coord.Y;
            }
        }
        bool operator >= (const Coord2D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() >= coord.taxicabDistance();
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() >= coord.euclideanDistance();
                case COORD_RELATE_XCOORD:
                    return X >= coord.X;
                case COORD_RELATE_YCOORD:
                    return Y >= coord.Y;
            }
        }
        unsigned char setRelationMetric(unsigned char metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (5 + metric % 5) % 5;
            return output;
        }
        unsigned char getRelationMetric() const {return RelationMetric;}

        Coord2D<ArithType> operator += (const Coord2D<ArithType> &coord) {
            Coord2D<ArithType> output(X, Y);
            X += coord.X;
            Y += coord.Y;
            return output;
        }
        Coord2D<ArithType> operator -= (const Coord2D<ArithType> &coord) {
            Coord2D<ArithType> output(X, Y);
            X -= coord.X;
            Y -= coord.Y;
            return output;
        }
        Coord2D<ArithType> operator *= (const ArithType &scalar) {
            Coord2D<ArithType> output(X, Y);
            X *= scalar;
            Y *= scalar;
            return output;
        }
        Coord2D<ArithType> operator + (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X + coord.X, Y + coord.Y);}
        Coord2D<ArithType> operator - (const Coord2D<ArithType> &coord) const {return Coord2D<ArithType>(X - coord.X, Y - coord.Y);}
        Coord2D<ArithType> operator * (const ArithType &scalar) const {return Coord2D<ArithType>(X * scalar, Y * scalar);}

        ArithType euclideanDistance(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) const {
            double distance = std::sqrt(std::pow(X - coord.X, 2) + std::pow(Y - coord.Y, 2));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType taxicabDistance(const Coord2D<ArithType> &coord = Coord2D<ArithType>(0, 0)) const {return std::fabs(X - coord.X) + std::fabs(Y - coord.Y);}
};

template <typename ArithType> class Coord3D {
    private:
        ArithType X = 0;
        ArithType Y = 0;
        ArithType Z = 0;
        unsigned char RelationMetric = COORD_RELATE_COMMON;

    public:
        Coord3D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord3D(ArithType x, ArithType y, ArithType z) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
            Z = z;
        }
        Coord3D(const Coord3D<ArithType> &coord) {
            X = coord.X;
            Y = coord.Y;
            Z = coord.Z;
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType getZ() const {return Z;}
        Coord3D<ArithType> setX(ArithType x) {
            Coord3D<ArithType> output(X, Y, Z);
            X = x;
            return output;
        }
        Coord3D<ArithType> setY(ArithType y) {
            Coord3D<ArithType> output(X, Y, Z);
            Y = y;
            return output;
        }
        Coord3D<ArithType> setZ(ArithType z) {
            Coord3D<ArithType> output(X, Y, Z);
            Z = z;
            return output;
        }

        std::vector<ArithType> toVector() const {return {X, Y, Z};}
        std::string toString(bool specifyPositive = false, short round = 3) const {return "(" + astr::toString(astr::round(X, round), specifyPositive) + ", " + astr::toString(astr::round(Y, round), specifyPositive) + ", " + astr::toString(astr::round(Z, round), specifyPositive) + ")";}
        std::string toString_Places(unsigned int beforeDecimal, unsigned int afterDecimal = 0, bool add = false, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Places(astr::round(X, round), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(Y, round), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(Z, round), beforeDecimal, afterDecimal, add, specifyPositive) + ")";}
        std::string toString_Length(unsigned int length, bool leading = true, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Length(astr::round(X, round), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(Y, round), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(Z, round), length, leading, specifyPositive) + ")";}
        std::string toString_Sci(short decimals = 2, short exponentDigits = 1, bool e = true, bool specifyPositive = false, short round = 3) const {return "(" + astr::toString_Sci(X, decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(Y, decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(Z, decimals, exponentDigits, e, specifyPositive) + ")";}

        Coord3D<ArithType> operator ! () const {return Coord3D<ArithType>(-X, -Y, -Z);}
        bool operator == (const Coord3D<ArithType> &coord) const {
            switch (RelationMetric) {
                default:
                case COORD_RELATE_COMMON:
                    return X == coord.X && Y == coord.Y && Z == coord.Z;
                case COORD_RELATE_EUCLID:
                    return euclideanDistance() == coord.euclideanDistance();
                case COORD_RELATE_TAXICB:
                    return taxicabDistance() == coord.taxicabDistance();
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
                case COORD_RELATE_TAXICB:
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
                case COORD_RELATE_TAXICB:
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
                case COORD_RELATE_TAXICB:
                case COORD_RELATE_TAXICB:
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
                case COORD_RELATE_TAXICB:
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
        unsigned char setRelationMetric(unsigned char metric = COORD_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (6 + metric % 6) % 6;
            return output;
        }
        unsigned char getRelationMetric() const {return RelationMetric;}

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

        ArithType euclideanDistance(const Coord3D<ArithType> &coord = Coord3D<ArithType>(0, 0, 0)) const {
            double distance = std::sqrt(std::pow(X - coord.X, 2) + std::pow(Y - coord.Y, 2) + std::pow(Z - coord.Z, 2));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType taxicabDistance(const Coord3D<ArithType> &coord = Coord3D<ArithType>(0, 0, 0)) const {return std::fabs(X - coord.X) + std::fabs(Y - coord.Y) + std::fabs(Z - coord.Z);}
};

#endif /* MATHCOORD */
