#ifndef VECTOR2D
#define VECTOR2D

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

template <typename ArithType> class Vector2D {
    private:
        ArithType M;
        double A;
        ArithType X;
        ArithType Y;

        static unsigned char RelationMetric;

        void calcCartesian() {
            if (std::is_integral<ArithType>::value) {
                X = std::round(M * std::cos(A));
                Y = std::round(M * std::sin(A));
                return;
            }
            X = M * std::cos(A);
            Y = M * std::sin(A);
        }
        void     calcPolar() {
            if (std::is_integral<ArithType>::value) {M = std::round(std::sqrt(X * X + Y * Y));}
            else {M = std::sqrt(X * X + Y * Y);}
            A = std::atan2(Y, X);
        }

    public:
        Vector2D() : M(1), A(0), X(1), Y(0) {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Vector2D(const ArithType &mag, const double &angle, const bool &useRadians) : M(mag), A(mapAngle(angle, useRadians)) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            calcCartesian();
        }
        Vector2D(const ArithType &x, const ArithType &y) : X(x), Y(y) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
            calcPolar();
        }
        Vector2D(const Vector2D<ArithType> &vector) : M(vector.getMag()), A(vector.getAngle()), X(vector.getX()), Y(vector.getY()) {}

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = VECTOR_RELATE_COMMON) {
            const unsigned char output = RelationMetric;
            RelationMetric = (metric > 4 || metric < 0) ? VECTOR_RELATE_COMMON : metric;
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
        double    getAngle(const bool &useRadians = true) const {return useRadians ? A : A * 180 / M_PI;}
        ArithType     getX()                              const {return X;}
        ArithType     getY()                              const {return Y;}
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
        double    setAngle(const double &angle) {
            const double output = A;
            A = mapAngle(angle);
            calcCartesian();
            return output;
        }
        double    adjAngle(const double &amount) {
            const double output = A;
            A = mapAngle(A + amount);
            calcCartesian();
            return output;
        }
        ArithType     setX(const ArithType &x) {
            const ArithType output = X;
            X = x;
            calcPolar();
            return output;
        }
        ArithType     adjX(const ArithType &amount) {
            const ArithType output = X;
            X += amount;
            calcPolar();
            return output;
        }
        ArithType     setY(const ArithType &y) {
            const ArithType output = Y;
            Y = y;
            calcPolar();
            return output;
        }
        ArithType     adjY(const ArithType &amount) {
            const ArithType output = Y;
            Y += amount;
            calcPolar();
            return output;
        }

        void operator=(const Vector2D<ArithType> &vector) {
            setMag(vector.getMag());
            setAngle(vector.getAngle());
            calcCartesian();
        }
        Vector2D<ArithType>          operator!() const {return Vector2D<ArithType>(M, A + M_PI);}
        std::string                   toString() const {return "(" + std::to_string(M) + ", " + std::to_string(A) + ")";}
        std::vector<ArithType>        toVector() const {return {X, Y};}
        std::pair<ArithType, double>    toPair() const {return {M, A};}

        bool        equal(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case VECTOR_RELATE_COMMON:
                    return M == vector.getMag() && A == vector.getAngle();
                case VECTOR_RELATE_MAGNITUDE:
                    return M == vector.getMag();
                case VECTOR_RELATE_THETA:
                    return A == vector.getAngle();
                case VECTOR_RELATE_XCOMP:
                    return X == vector.getX();
                case VECTOR_RELATE_YCOMP:
                    return Y == vector.getY();
            }
        }
        bool     notequal(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {return !equal(vector, metric);}
        bool         less(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case VECTOR_RELATE_COMMON:
                case VECTOR_RELATE_MAGNITUDE:
                    return M < vector.getMag();
                case VECTOR_RELATE_THETA:
                    return A < vector.getAngle();
                case VECTOR_RELATE_XCOMP:
                    return X < vector.getX();
                case VECTOR_RELATE_YCOMP:
                    return Y < vector.getY();
            }
        }
        bool      greater(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {return vector.less(*this, metric);}
        bool    lessequal(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {return !greater(vector, metric);}
        bool greaterequal(const Vector2D<ArithType> &vector, const unsigned char &metric = RelationMetric) const {return !less(vector, metric);}
        bool   operator==(const Vector2D<ArithType> &vector) const {return        equal(vector, RelationMetric);}
        bool   operator!=(const Vector2D<ArithType> &vector) const {return     notequal(vector, RelationMetric);}
        bool    operator<(const Vector2D<ArithType> &vector) const {return         less(vector, RelationMetric);}
        bool    operator>(const Vector2D<ArithType> &vector) const {return      greater(vector, RelationMetric);}
        bool   operator<=(const Vector2D<ArithType> &vector) const {return    lessequal(vector, RelationMetric);}
        bool   operator>=(const Vector2D<ArithType> &vector) const {return greaterequal(vector, RelationMetric);}

        Vector2D<ArithType>& operator+=(const Vector2D<ArithType> &vector) {
            X += vector.getX();
            Y += vector.getY();
            calcPolar();
            return *this;
        }
        Vector2D<ArithType>& operator-=(const Vector2D<ArithType> &vector) {
            X -= vector.getX();
            Y -= vector.getY();
            calcPolar();
            return *this;
        }
        Vector2D<ArithType>& operator*=(const ArithType &scalar) {
            X *= scalar;
            Y *= scalar;
            calcPolar();
            return *this;
        }
        Vector2D<ArithType>& operator/=(const ArithType &scalar) {
            X /= scalar;
            Y /= scalar;
            calcPolar();
            return *this;
        }
        Vector2D<ArithType>& operator%=(const ArithType &denom)  {
            X = std::fmod(X, denom);
            Y = std::fmod(Y, denom);
            calcPolar();
            return *this;
        }
        Vector2D<ArithType>   operator+(const Vector2D<ArithType> &vector) const {return Vector2D<ArithType>(X + vector.getX(), Y + vector.getY());}
        Vector2D<ArithType>   operator-(const Vector2D<ArithType> &vector) const {return Vector2D<ArithType>(X - vector.getX(), Y - vector.getY());}
        Vector2D<ArithType>   operator*(const ArithType &scalar)           const {return Vector2D<ArithType>(X * scalar, Y * scalar);}
        Vector2D<ArithType>   operator/(const ArithType &scalar)           const {return Vector2D<ArithType>(X / scalar, Y / scalar);}
        Vector2D<ArithType>   operator%(const ArithType &denom)            const {return Vector2D<ArithType>(std::fmod(X, denom), std::fmod(Y, denom));}

        ArithType            dot(const Vector2D<ArithType> &vector) const {return X * vector.getX() + Y * vector.getY();}
        ArithType          cross(const Vector2D<ArithType> &vector) const {return X * vector.getY() - Y * vector.getX();} 
        Vector2D<ArithType> unit()                                  const {return Vector2D<ArithType>(1, A);}
};

template <typename ArithType> unsigned char Vector2D<ArithType>::RelationMetric = VECTOR_RELATE_COMMON;

#endif /* VECTOR2D */
