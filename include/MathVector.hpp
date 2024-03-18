#ifndef MATHVECTOR
#define MATHVECTOR

#include <cmath>
#include <vector>
#include <string>

#include "astr.hpp"

#define VECTOR_RELATE_COMMON 0
#define VECTOR_RELATE_MAG 1
#define VECTOR_RELATE_THETA 2
#define VECTOR_RELATE_PHI 3

template <typename ArithType> class Vector2D {
    private:
        ArithType X = 0;
        ArithType Y = 0;
        ArithType M = 0;
        double T = 0;
        unsigned char RelationMetric = VECTOR_RELATE_COMMON;

        void calcCartesian() {
            if (std::is_integral<ArithType>::value) {
                X = std::round(M * std::cos(T));
                Y = std::round(M * std::sin(T));
                return;
            }
            X = M * std::cos(T);
            Y = M * std::sin(T);
        }
        void calcPolar() {
            double m = std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
            if (std::is_integral<ArithType>::value) {M = std::round(m);}
            M = m;

            T = std::atan2(Y, X);
        }

    public:
        Vector2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Vector2D(ArithType x, ArithType y) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
            calcPolar();
        }
        Vector2D(ArithType magnitude, double theta, bool degrees) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            M = magnitude;
            T = theta;

            if (degrees) {T *= M_PI / 180;}
            calcCartesian();
        }
        Vector2D(const Vector2D<ArithType> &vector) {
            X = vector.X;
            Y = vector.Y;
            M = vector.M;
            T = vector.T;
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType getMag() const {return M;}
        double getAngle(bool degrees = true) const {return degrees ? T * 180 / M_PI: T;}
        Vector2D<ArithType> setX(ArithType x) {
            Vector2D<ArithType> output(X, Y);
            X = x;
            calcPolar();
            return output;
        }
        Vector2D<ArithType> setY(ArithType y) {
            Vector2D<ArithType> output(X, Y);
            Y = y;
            calcPolar();
            return output;
        }
        Vector2D<ArithType> setMag(ArithType magnitude) {
            Vector2D<ArithType> output(M, T, false);
            M = magnitude;
            calcCartesian();
            return output;
        }
        Vector2D<ArithType> setAngle(double theta, bool degrees = true) {
            Vector2D<ArithType> output(M, T, false);
            T = std::fmod(theta * (degrees ? M_PI / 180 : 1), 2 * M_PI);
            calcCartesian();
            return output;
        }

        Vector2D<ArithType> sToUnitVector() const {return Vector2D<ArithType>(1, T, false);}
        Vector2D<ArithType> rToUnitVector() const {return Vector2D<ArithType>(X / M, Y / M);}

        std::string toString(bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString(astr::round(M), specifyPositive) + ", " + astr::toString(astr::round(T * (degrees ? 180 / M_PI : 1)), specifyPositive) + ")";}
        std::string toString_Places(unsigned int beforeDecimal, unsigned int afterDecimal = 0, bool add = false, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Places(astr::round(M), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(T * (degrees ? 180 / M_PI : 1)), beforeDecimal, afterDecimal, add, specifyPositive) + ")";}
        std::string toString_Length(unsigned int length, bool leading = true, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Length(astr::round(M), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(T * (degrees ? 180 / M_PI : 1)), length, leading, specifyPositive) + ")";}
        std::string toString_Sci(short decimals = 2, short exponentDigits = 1, bool e = true, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Sci(M, decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(astr::round(T * (degrees ? 180 / M_PI : 1)), decimals, exponentDigits, e, specifyPositive) + ")";}

        Vector2D<ArithType> operator ! () const {return Vector2D<ArithType>(-X, -Y);}
        bool operator == (const Vector2D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_COMMON:
                    return M == vector.M && T == vector.T;
                case VECTOR_RELATE_MAG:
                    return M == vector.M;
                case VECTOR_RELATE_THETA:
                    return T == vector.T;
            }
        }
        bool operator != (const Vector2D<ArithType> &vector) const {return !(Vector2D<ArithType>(M, T, false) == vector);}
        bool operator < (const Vector2D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M < vector.M;
                case VECTOR_RELATE_THETA:
                    return T < vector.T;
            }
        }
        bool operator <= (const Vector2D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M <= vector.M;
                case VECTOR_RELATE_THETA:
                    return T <= vector.T;
            }
        }
        bool operator > (const Vector2D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M > vector.M;
                case VECTOR_RELATE_THETA:
                    return T > vector.T;
            }
        }
        bool operator >= (const Vector2D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M >= vector.M;
                case VECTOR_RELATE_THETA:
                    return T >= vector.T;
            }
        }
        unsigned char setRelationMetric(unsigned char metric = VECTOR_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (3 + metric % 3) % 3;
            return output;
        }
        unsigned char getRelationMetric() const {return RelationMetric;}

        Vector2D<ArithType> operator += (const Vector2D<ArithType> &vector) {
            Vector2D<ArithType> output(X, Y);
            X += vector.X;
            Y += vector.Y;
            calcPolar();
            return output;
        }
        Vector2D<ArithType> operator -= (const Vector2D<ArithType> &vector) {
            Vector2D<ArithType> output(X, Y);
            X -= vector.X;
            Y -= vector.Y;
            calcPolar();
            return output;
        }
        Vector2D<ArithType> operator *= (const ArithType &scalar) {
            Vector2D<ArithType> output(X, Y);
            X *= scalar;
            Y *= scalar;
            calcPolar();
            return output;
        }
        Vector2D<ArithType> operator + (const Vector2D<ArithType> &vector) {return Vector2D<ArithType>(X + vector.X, Y + vector.Y);}
        Vector2D<ArithType> operator - (const Vector2D<ArithType> &vector) {return Vector2D<ArithType>(X - vector.X, Y - vector.Y);}
        Vector2D<ArithType> operator * (const ArithType &scalar) {return Vector2D<ArithType>(X * scalar, Y * scalar);}
};

template <typename ArithType> class Vector3D {
    private:
        ArithType X = 0;
        ArithType Y = 0;
        ArithType Z = 0;
        ArithType M = 0;
        double T = 0;
        double P = 0;
        unsigned char RelationMetric = VECTOR_RELATE_COMMON;

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
            double m = std::sqrt(pow(X, 2) + std::pow(Y, 2) + std::pow(Z, 2));
            if (std::is_integral<ArithType>::value) {M = std::round(m);}
            M = m;

            T = std::acos(Z / m);
            P = std::acos(X / std::sqrt(std::pow(X, 2) + std::pow(Y, 2)));
        }

    public:
        Vector3D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Vector3D(ArithType x, ArithType y, ArithType z) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            X = x;
            Y = y;
            Z = z;
            calcSpherical();
        }
        Vector3D(ArithType magnitude, double theta, double phi, bool degrees) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            M = magnitude;
            T = theta;
            P = phi;

            if (degrees) {
                T *= M_PI / 180;
                P *= M_PI / 180;
            }
            calcCartesian();
        }
        Vector3D(const Vector3D<ArithType> &vector) {
            X = vector.X;
            Y = vector.Y;
            Z = vector.Z;
            M = vector.M;
            T = vector.T;
            P = vector.P;
        }

        ArithType getX() const {return X;}
        ArithType getY() const {return Y;}
        ArithType getZ() const {return Z;}
        ArithType getMag() const {return M;}
        double getTheta(bool degrees = true) const {return degrees ? T * 180 / M_PI: T;}
        double getPhi(bool degrees = true) const {return degrees ? P * 180 / M_PI: P;}
        Vector3D<ArithType> setX(ArithType x) {
            Vector3D<ArithType> output(X, Y, Z);
            X = x;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> setY(ArithType y) {
            Vector3D<ArithType> output(X, Y, Z);
            Y = y;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> setZ(ArithType z) {
            Vector3D<ArithType> output(X, Y, Z);
            Z = z;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> setMag(ArithType magnitude) {
            Vector3D<ArithType> output(M, T, P, false);
            M = magnitude;
            calcCartesian();
            return output;
        }
        Vector3D<ArithType> setTheta(double theta, bool degrees = true) {
            Vector3D<ArithType> output(M, T, P, false);
            T = theta * (degrees ? M_PI / 180 : 1);
            calcCartesian();
            return output;
        }
        Vector3D<ArithType> setPhi(double phi, bool degrees = true) {
            Vector3D<ArithType> output(M, T, P, false);
            P = phi * (degrees ? M_PI / 180 : 1);
            calcCartesian();
            return output;
        }

        Vector3D<ArithType> sToUnitVector() const {return Vector3D<ArithType>(1, T, P, false);}
        Vector3D<ArithType> rToUnitVector() const {return Vector3D<ArithType>(X / M, Y / M, Z / M);}

        std::string toString(bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString(astr::round(M), specifyPositive) + ", " + astr::toString(astr::round(T * (degrees ? 180 / M_PI : 1)), specifyPositive) + ", " + astr::toString(astr::round(P * (degrees ? 180 / M_PI : 1)), specifyPositive) + ")";}
        std::string toString_Places(unsigned int beforeDecimal, unsigned int afterDecimal = 0, bool add = false, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Places(astr::round(M), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(T * (degrees ? 180 / M_PI : 1)), beforeDecimal, afterDecimal, add, specifyPositive) + ", " + astr::toString_Places(astr::round(P * (degrees ? 180 / M_PI : 1)), beforeDecimal, afterDecimal, add, specifyPositive) + ")";}
        std::string toString_Length(unsigned int length, bool leading = true, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Length(astr::round(M), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(T * (degrees ? 180 / M_PI : 1)), length, leading, specifyPositive) + ", " + astr::toString_Length(astr::round(P * (degrees ? 180 / M_PI : 1)), length, leading, specifyPositive) + ")";}
        std::string toString_Sci(short decimals = 2, short exponentDigits = 1, bool e = true, bool specifyPositive = false, short round = 3, bool degrees = true) const {return "(" + astr::toString_Sci(M, decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(astr::round(T * (degrees ? 180 / M_PI : 1)), decimals, exponentDigits, e, specifyPositive) + ", " + astr::toString_Sci(astr::round(P * (degrees ? 180 / M_PI : 1)), decimals, exponentDigits, e, specifyPositive) + ")";}

        Vector3D<ArithType> operator ! () const {return Vector3D<ArithType>(-X, -Y, -Z);}
        bool operator == (const Vector3D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_COMMON:
                    return M == vector.M && T == vector.T && P == vector.P;
                case VECTOR_RELATE_MAG:
                    return M == vector.M;
                case VECTOR_RELATE_THETA:
                    return T == vector.T;
                case VECTOR_RELATE_PHI:
                    return P == vector.P;
            }
        }
        bool operator != (const Vector3D<ArithType> &vector) const {return !(Vector3D<ArithType>(M, T, P, false) == vector);}
        bool operator < (const Vector3D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M < vector.M;
                case VECTOR_RELATE_THETA:
                    return T < vector.T;
                case VECTOR_RELATE_PHI:
                    return P < vector.P;
            }
        }
        bool operator <= (const Vector3D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M <= vector.M;
                case VECTOR_RELATE_THETA:
                    return T <= vector.T;
                case VECTOR_RELATE_PHI:
                    return P <= vector.P;
            }
        }
        bool operator > (const Vector3D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M > vector.M;
                case VECTOR_RELATE_THETA:
                    return T > vector.T;
                case VECTOR_RELATE_PHI:
                    return P > vector.P;
            }
        }
        bool operator >= (const Vector3D<ArithType> &vector) const {
            switch (RelationMetric) {
                default:
                case VECTOR_RELATE_MAG:
                case VECTOR_RELATE_COMMON:
                    return M >= vector.M;
                case VECTOR_RELATE_THETA:
                    return T >= vector.T;
                case VECTOR_RELATE_PHI:
                    return P >= vector.P;
            }
        }
        unsigned char setRelationMetric(unsigned char metric = VECTOR_RELATE_COMMON) {
            unsigned char output = RelationMetric;
            RelationMetric = (4 + metric % 4) % 4;
            return output;
        }
        unsigned char getRelationMetric() const {return RelationMetric;}

        Vector3D<ArithType> operator += (const Vector3D<ArithType> &vector) {
            Vector3D<ArithType> output(X, Y, Z);
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> operator -= (const Vector3D<ArithType> &vector) {
            Vector3D<ArithType> output(X, Y, Z);
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> operator *= (const ArithType &scalar) {
            Vector3D<ArithType> output(X, Y, Z);
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            calcSpherical();
            return output;
        }
        Vector3D<ArithType> operator + (const Vector3D<ArithType> &vector) {return Vector3D<ArithType>(X + vector.X, Y + vector.Y, Z + vector.Z);}
        Vector3D<ArithType> operator - (const Vector3D<ArithType> &vector) {return Vector3D<ArithType>(X - vector.X, Y - vector.Y, Z - vector.Z);}
        Vector3D<ArithType> operator * (const ArithType &scalar) {return Vector3D<ArithType>(X * scalar, Y * scalar, Z * scalar);}
};

#endif /* MATHVECTOR */
