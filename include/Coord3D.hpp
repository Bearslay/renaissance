#ifndef COORD3D
#define COORD3D

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

#ifndef COORD3D_RELATIONS
#define COORD3D_RELATIONS
#define COORD_RELATE_ZCOORD 5
#endif /* COORD3D_RELATIONS */

template <typename ArithType> class Coord3D {
    private:
        ArithType X;
        ArithType Y;
        ArithType Z;

        static unsigned char RelationMetric;
        static Coord3D<ArithType> ReferencePoint;

    public:
        Coord3D() : X(0), Y(0), Z(0) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord3D(const ArithType &x, const ArithType &y, const ArithType &z) : X(x), Y(y), Z(z) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Coord3D(const Coord3D<ArithType> &coord) : X(coord.getX()), Y(coord.getY()), Z(coord.getZ()) {}

        static unsigned char      getRelationMetric() {return RelationMetric;}
        static unsigned char      setRelationMetric(const unsigned char &metric = COORD_RELATE_COMMON) {
            const unsigned char output = RelationMetric;
            RelationMetric = (metric > 5 || metric < 0) ? COORD_RELATE_COMMON : metric;
            return output;
        }
        static Coord3D<ArithType> getReferencePoint() {return ReferencePoint;}
        static Coord3D<ArithType> setReferencePoint(const Coord3D<ArithType> &coord = Coord3D<ArithType>(0, 0, 0)) {
            const Coord3D<ArithType> output(ReferencePoint.getX(), ReferencePoint.getY(), ReferencePoint.getZ());
            ReferencePoint = coord;
            return output;
        }
        static Coord3D<ArithType> setReferencePoint(const ArithType &x, const ArithType &y, const ArithType &z) {return setReferencePoint(Coord3D<ArithType>(x, y, z));}

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType getZ() const {return Z;}
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
        ArithType setZ(const ArithType &z) {
            const ArithType output = Z;
            Z = z;
            return output;
        }
        ArithType adjZ(const ArithType &amount) {
            const ArithType output = Z;
            Z += amount;
            return output;
        }

        Coord3D<ArithType> reflectX() {
            const Coord3D<ArithType> output = *this;
            Y = -Y;
            return output;
        }
        Coord3D<ArithType> reflectY() {
            const Coord3D<ArithType> output = *this;
            X = -X;
            return output;
        }
        Coord3D<ArithType> reflectZ() {
            const Coord3D<ArithType> output = *this;
            Z = -Z;
            return output;
        }
        // TODO: Add a reflection for any line
        Coord3D<ArithType> stretchX(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            X *= scalar;
            return output;
        }
        Coord3D<ArithType>  shrinkX(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            X /= scalar;
            return output;
        }
        Coord3D<ArithType> stretchY(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            Y *= scalar;
            return output;
        }
        Coord3D<ArithType>  shrinkY(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            Y /= scalar;
            return output;
        }
        Coord3D<ArithType> stretchZ(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            Z *= scalar;
            return output;
        }
        Coord3D<ArithType>  shrinkZ(const ArithType &scalar) {
            const Coord3D<ArithType> output = *this;
            Z /= scalar;
            return output;
        }
        // TODO: Add a stretch along any line
        Coord3D<ArithType>  rotateX(const Coord3D<ArithType> &pivot, const double &angle) {
            const Coord3D<ArithType> output = *this;
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            Y = (Y - pivot.getY()) * c - (Z - pivot.getZ()) * s + pivot.getY();
            Z = (Y - pivot.getY()) * s + (Z - pivot.getZ()) * c + pivot.getZ();
            return output;
        }
        Coord3D<ArithType>  rotateX(const double &angle) const {return rotateX(ReferencePoint, angle);}
        Coord3D<ArithType>  rotateY(const Coord3D<ArithType> &pivot, const double &angle) {
            const Coord3D<ArithType> output = *this;
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            X = (X - pivot.getX()) * c - (Z - pivot.getZ()) * s + pivot.getX();
            Z = (X - pivot.getX()) * s + (Z - pivot.getZ()) * c + pivot.getZ();
            return output;
        }
        Coord3D<ArithType>  rotateY(const double &angle) const {return rotateY(ReferencePoint, angle);}
        Coord3D<ArithType>  rotateZ(const Coord3D<ArithType> &pivot, const double &angle) {
            const Coord3D<ArithType> output = *this;
            const double s = std::sin(angle);
            const double c = std::cos(angle);
            X = (X - pivot.getx()) * c - (Y - pivot.gety()) * s + pivot.getX();
            Y = (X - pivot.getX()) * s + (Y - pivot.getY()) * c + pivot.getY();
            return output;
        }
        Coord3D<ArithType>  rotateZ(const double &angle) const {return rotateZ(ReferencePoint, angle);}
        Coord3D<ArithType>   rotate(const Coord3D<ArithType> &pivot, const double &theta = M_PI_2, const double &phi = 0) {
            const Coord3D<ArithType> output = *this;
            const double st = std::sin(theta);
            const double ct = std::cos(theta);
            const double sp = std::sin(phi);
            const double cp = std::cos(phi);
            X -= pivot.getX();
            Y -= pivot.getY();
            Z -= pivot.getZ();
            X = X * ct - Z * st;
            Z = X * st + Z * ct;
            X += pivot.getX();
            Y += pivot.getY();
            Z += pivot.getZ();
            return output;
        }
        Coord3D<ArithType>   rotate(const double &theta = M_PI_2) {return rotate(ReferencePoint, theta);}

        void operator=(const Coord3D<ArithType> &coord) {
            setX(coord.getX());
            setY(coord.getY());
            setZ(coord.getZ());
        }
        Coord3D<ArithType>     operator!() const {return Coord3D<ArithType>(-X, -Y, -Z);}
        std::string             toString() const {return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";}
        std::vector<ArithType>  toVector() const {return {X, Y, Z};}

        bool        equal(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case COORD_RELATE_COMMON:
                    return X == coord.getX() && Y == coord.getY() && Z == coord.getZ();
                case COORD_RELATE_EUCLID:
                    return distEuclid() == coord.distEuclid();
                case COORD_RELATE_TAXICAB:
                    return distTaxi() == coord.distTaxi();
                case COORD_RELATE_XCOORD:
                    return X == coord.getX();
                case COORD_RELATE_YCOORD:
                    return Y == coord.getY();
                case COORD_RELATE_ZCOORD:
                    return Z == coord.getZ();
            }
        }
        bool     notequal(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {return !equal(coord, metric);}
        bool         less(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {
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
                case COORD_RELATE_ZCOORD:
                    return Z < coord.getZ();
            }
        }
        bool      greater(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {return coord.less(*this, metric);}
        bool    lessequal(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {return !greater(coord, metric);}
        bool greaterequal(const Coord3D<ArithType> &coord, const unsigned char &metric = RelationMetric) const {return !less(coord, metric);}
        bool   operator==(const Coord3D<ArithType> &coord) const {return        equal(coord, RelationMetric);}
        bool   operator!=(const Coord3D<ArithType> &coord) const {return     notequal(coord, RelationMetric);}
        bool    operator<(const Coord3D<ArithType> &coord) const {return         less(coord, RelationMetric);}
        bool    operator>(const Coord3D<ArithType> &coord) const {return      greater(coord, RelationMetric);}
        bool   operator<=(const Coord3D<ArithType> &coord) const {return    lessequal(coord, RelationMetric);}
        bool   operator>=(const Coord3D<ArithType> &coord) const {return greaterequal(coord, RelationMetric);}

        Coord3D<ArithType>& operator+=(const Coord3D<ArithType> &coord) {
            X += coord.getX();
            Y += coord.getY();
            Z += coord.getZ();
            return *this;
        }
        Coord3D<ArithType>& operator-=(const Coord3D<ArithType> &coord) {
            X -= coord.getX();
            Y -= coord.getY();
            Z -= coord.getZ();
            return *this;
        }
        Coord3D<ArithType>& operator*=(const ArithType &scalar) {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            return *this;
        }
        Coord3D<ArithType>& operator/=(const ArithType &scalar) {
            X /= scalar;
            Y /= scalar;
            Z /= scalar;
            return *this;
        }
        Coord3D<ArithType>& operator%=(const ArithType &denom)  {
            X = std::fmod(X, denom);
            Y = std::fmod(Y, denom);
            Z = std::fmod(Z, denom);
            return *this;
        }
        Coord3D<ArithType>   operator+(const Coord3D<ArithType> &coord) const {return Coord3D<ArithType>(X + coord.getX(), Y + coord.getY(), Z + coord.getZ());}
        Coord3D<ArithType>   operator-(const Coord3D<ArithType> &coord) const {return Coord3D<ArithType>(X - coord.getX(), Y - coord.getY(), Z - coord.getZ());}
        Coord3D<ArithType>   operator*(const ArithType &scalar)         const {return Coord3D<ArithType>(X * scalar, Y * scalar, Z * scalar);}
        Coord3D<ArithType>   operator/(const ArithType &scalar)         const {return Coord3D<ArithType>(X / scalar, Y / scalar, Z / scalar);}
        Coord3D<ArithType>   operator%(const ArithType &denom)          const {return Coord3D<ArithType>(std::fmod(X, denom), std::fmod(Y, denom), std::fmod(Z, denom));}

        ArithType distEuclid(const Coord3D<ArithType> &coord) const {
            const double distance = std::sqrt((X - coord.getX()) * (X - coord.getX()) + (Y - coord.getY()) * (Y - coord.getY()) + (Z - coord.getZ()) * (Z - coord.getZ()));
            return std::is_integral<ArithType>::value ? std::round(distance) : distance;
        }
        ArithType distEuclid()                                const {return distEuclid(ReferencePoint);}
        ArithType   distTaxi(const Coord3D<ArithType> &coord) const {return std::fabs(X - coord.getX()) + std::fabs(Y - coord.getY() + std::fabs(Z - coord.getZ()));}
        ArithType   distTaxi()                                const {return distTaxi(ReferencePoint);}
};

template <typename ArithType> unsigned char Coord3D<ArithType>::RelationMetric = COORD_RELATE_COMMON;
template <typename ArithType> Coord3D<ArithType> Coord3D<ArithType>::ReferencePoint = Coord3D<ArithType>(0, 0, 0);

#endif /* COORD3D */
