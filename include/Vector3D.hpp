#ifndef VECTOR3D
#define VECTOR3D

#include <cmath>
#include <vector>
#include <utility>
#include <string>

#ifndef VECTOR2D_RELATIONS
#define VECTOR2D_RELATIONS
#define VECTOR_RELATE_COMMON 0
#define VECTOR_RELATE_MAGNITUDE 1
#define VECTOR_RELATE_THETA 2
#define VECTOR_RELATE_XCOMP 3
#define VECTOR_RELATE_YCOMP 4
#endif /* VECTOR2D_RELATIONS */

#ifndef VECTOR3D_RELATIONS
#define VECTOR3D_RELATIONS
#define VECTOR_RELATE_PHI 5
#define VECTOR_RELATE_ZCOMP 6
#endif /* COORD3D_RELATIONS */

template <typename ArithType> class Vector3D {
    private:
        ArithType M;
        double T;
        double P;
        ArithType X;
        ArithType Y;
        ArithType Z;

        static unsigned char RelationMetric;

        void calcCartesian() {
            if (std::is_integral<ArithType>::value) {
                X = std::round(M * std::sin(P) * std::cos(T));
                Y = std::round(M * std::sin(P) * std::sin(T));
                Z = std::round(M * std::cos(P));
                return;
            }
            X = M * std::sin(P) * std::cos(T);
            Y = M * std::sin(P) * std::sin(T);
            Z = M * std::cos(P); 
        }
        void calcSpherical() {
            const double m = std::sqrt(X * X + Y * Y + Z * Z);
            if (std::is_integral<ArithType>::value) {M = std::round(m);}
            else {M = m;}
            T = std::atan2(Z, X);
            P = std::atan2(Y, X);
        }

    public:
        Vector3D() : M(1), T(0), P(0), X(1), Y(0), Z(0) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Vector3D(const ArithType &mag, const double &theta, const double &phi, const bool &useRadians) : M(mag), T(mapAngle(theta, useRadians)), P(mapAngle(phi, useRadians)) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            calcCartesian();
        }
        Vector3D(const ArithType &x, const ArithType &y, const ArithType &z) : X(x), Y(y), Z(z) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            calcSpherical();
        }
        Vector3D(const Vector3D<ArithType> &vector) : M(vector.getMag()), T(vector.getTheta()), P(vector.getPhi()), X(vector.getX()), Y(vector.getY()), Z(vector.getZ()) {}

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = VECTOR_RELATE_COMMON) {
            const unsigned char output = RelationMetric;
            RelationMetric = (metric > 6 || metric < 0) ? VECTOR_RELATE_COMMON : metric;
            return output;
        }
        static double mapAngle(const double &angle, const bool &useRadians = true) {
            if (useRadians) {
                if (angle > 0 && angle < 2 * M_PI) {return angle;}

                double output = angle;
                output = std::fmod(output, 2 * M_PI);
                if (output < 0) {output += 2 * M_PI;}
                return output;
            }
            if (angle > 0 && angle < 360) {return angle;}

            double output = angle;
            output = std::fmod(output, 360);
            if (output < 0) {output += 360;}
            return output;
        }

        ArithType   getMag()                              const {return M;}
        double    getTheta(const bool &useRadians = true) const {return useRadians ? T : T * 180 / M_PI;}
        double      getPhi(const bool &useRadians = true) const {return useRadians ? P : P * 180 / M_PI;}
        ArithType     getX()                              const {return X;}
        ArithType     getY()                              const {return Y;}
        ArithType     getZ()                              const {return Z;}
        ArithType   setMag(const ArithType &mag) {
            const ArithType output = M;
            M = mag;
            calcCartesian();
            return output;
        }
        ArithType   adjMag(const ArithType &amount) {
            const ArithType output = M;
            M += amount;
            calcCartesian();
            return output;
        }
        double    setTheta(const double &theta) {
            const double output = T;
            T = mapAngle(theta);
            calcCartesian();
            return output;
        }
        double    adjTheta(const double &amount) {
            const double output = T;
            T = mapAngle(T + amount);
            calcCartesian();
            return output;
        }
        double      setPhi(const double &phi) {
            const double output = P;
            P = mapAngle(phi);
            calcCartesian();
            return output;
        }
        double      adjPhi(const double &amount) {
            const double output = P;
            P = mapAngle(P + amount);
            calcCartesian();
            return output;
        }
        ArithType     setX(const ArithType &x) {
            const ArithType output = X;
            X = x;
            calcSpherical();
            return output;
        }
        ArithType     adjX(const ArithType &amount) {
            const ArithType output = X;
            X += amount;
            calcSpherical();
            return output;
        }
        ArithType     setY(const ArithType &y) {
            const ArithType output = Y;
            Y = y;
            calcSpherical();
            return output;
        }
        ArithType     adjY(const ArithType &amount) {
            const ArithType output = Y;
            Y += amount;
            calcSpherical();
            return output;
        }
        ArithType     setZ(const ArithType &z) {
            const ArithType output = Z;
            Z = z;
            calcSpherical();
            return output;
        }
        ArithType     adjZ(const ArithType &amount) {
            const ArithType output = Z;
            Z += amount;
            calcSpherical();
            return output;
        }

        void operator=(const Vector3D<ArithType> &vector) {
            setMag(vector.getMag());
            setTheta(vector.getTheta());
            setPhi(vector.getPhi());
            calcCartesian();
        }
        Vector3D<ArithType>                       operator!() const {return Vector3D<ArithType>(M, T + M_PI, P + M_PI);}
        std::string                                toString() const {return "(" + std::to_string(M) + ", " + std::to_string(T) + ", " + std::to_string(P) + ")";}
        std::vector<ArithType>                     toVector() const {return {X, Y, Z};}
        std::pair<ArithType, std::vector<double>>    toPair() const {return {M, {T, P}};}

        bool        equal(const Vector3D<ArithType> &vector, const unsigned char &metric) const {
            switch (metric) {
                default:
                case VECTOR_RELATE_COMMON:
                    return M == vector.getMag() && T == vector.getTheta() && P == vector.getPhi();
                case VECTOR_RELATE_MAGNITUDE:
                    return M == vector.getMag();
                case VECTOR_RELATE_THETA:
                    return T == vector.getTheta();
                case VECTOR_RELATE_PHI:
                    return P == vector.getPhi();
                case VECTOR_RELATE_XCOMP:
                    return X == vector.getX();
                case VECTOR_RELATE_YCOMP:
                    return Y == vector.getY();
                case VECTOR_RELATE_ZCOMP:
                    return Z == vector.getZ();
            }
        }
        bool     notequal(const Vector3D<ArithType> &vector, const unsigned char &metric) const {return !equal(vector, metric);}
        bool         less(const Vector3D<ArithType> &vector, const unsigned char &metric) const {
            switch (metric) {
                default:
                case VECTOR_RELATE_COMMON:
                case VECTOR_RELATE_MAGNITUDE:
                    return M < vector.getMag();
                case VECTOR_RELATE_THETA:
                    return T < vector.getTheta();
                case VECTOR_RELATE_PHI:
                    return P < vector.getPhi();
                case VECTOR_RELATE_XCOMP:
                    return X < vector.getX();
                case VECTOR_RELATE_YCOMP:
                    return Y < vector.getY();
                case VECTOR_RELATE_ZCOMP:
                    return Z < vector.getZ();
            }
        }
        bool      greater(const Vector3D<ArithType> &vector, const unsigned char &metric) const {return vector.less(*this, metric);}
        bool    lessequal(const Vector3D<ArithType> &vector, const unsigned char &metric) const {return !greater(vector, metric);}
        bool greaterequal(const Vector3D<ArithType> &vector, const unsigned char &metric) const {return !less(vector, metric);}
        bool   operator==(const Vector3D<ArithType> &vector) const {return        equal(vector, RelationMetric);}
        bool   operator!=(const Vector3D<ArithType> &vector) const {return     notequal(vector, RelationMetric);}
        bool    operator<(const Vector3D<ArithType> &vector) const {return         less(vector, RelationMetric);}
        bool    operator>(const Vector3D<ArithType> &vector) const {return      greater(vector, RelationMetric);}
        bool   operator<=(const Vector3D<ArithType> &vector) const {return    lessequal(vector, RelationMetric);}
        bool   operator>=(const Vector3D<ArithType> &vector) const {return greaterequal(vector, RelationMetric);}

        Vector3D<ArithType>& operator+=(const Vector3D<ArithType> &vector) {
            X += vector.getX();
            Y += vector.getY();
            Z += vector.getZ();
            calcSpherical();
            return *this;
        }
        Vector3D<ArithType>& operator-=(const Vector3D<ArithType> &vector) {
            X -= vector.getX();
            Y -= vector.getY();
            Z -= vector.getZ();
            calcSpherical();
            return *this;
        }
        Vector3D<ArithType>& operator*=(const ArithType &scalar) {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            calcSpherical();
            return *this;
        }
        Vector3D<ArithType>& operator/=(const ArithType &scalar) {
            X /= scalar;
            Y /= scalar;
            Z /= scalar;
            calcSpherical();
            return *this;
        }
        Vector3D<ArithType>& operator%=(const ArithType &denom)  {
            X = std::fmod(X, denom);
            Y = std::fmod(Y, denom);
            Z = std::fmod(Z, denom);
            calcSpherical();
            return *this;
        }
        Vector3D<ArithType>   operator+(const Vector3D<ArithType> &vector) const {return Vector3D<ArithType>(X + vector.getX(), Y + vector.getY(), Z + vector.getZ());}
        Vector3D<ArithType>   operator-(const Vector3D<ArithType> &vector) const {return Vector3D<ArithType>(X - vector.getX(), Y - vector.getY(), Z - vector.getZ());}
        Vector3D<ArithType>   operator*(const ArithType &scalar)           const {return Vector3D<ArithType>(X * scalar, Y * scalar, Z * scalar);}
        Vector3D<ArithType>   operator/(const ArithType &scalar)           const {return Vector3D<ArithType>(X / scalar, Y / scalar, Z / scalar);}
        Vector3D<ArithType>   operator%(const ArithType &denom)            const {return Vector3D<ArithType>(std::fmod(X, denom), std::fmod(Y, denom), std::fmod(Z, denom));}

        ArithType             dot(const Vector3D<ArithType> &vector) const {return X * vector.getX() + Y * vector.getY() + Z * vector.getZ();}
        Vector3D<ArithType> cross(const Vector3D<ArithType> &vector) const {return Vector3D<ArithType>(Y * vector.getZ() - Z * vector.getY(), Z * vector.getX() - X * vector.getZ(), X * vector.getY() - Y * vector.getX());}
        Vector3D<ArithType>  unit()                                  const {return Vector3D<ArithType>(1, T, P);}
};

template <typename ArithType> unsigned char Vector3D<ArithType>::RelationMetric = VECTOR_RELATE_COMMON;

#endif /* VECTOR2D */
