#ifndef UTILITIES
#define UTILITIES

#include <cmath>

/* pi/8 rad or 22.5 deg */
#define C_PI_8     0.39269908169872415481
/* pi/6 rad or 30 deg */
#define C_PI_6     0.52359877559829887308
/* pi/4 rad or 45 deg */
#define C_PI_4     0.78539816339744830961
/* pi/3 rad or 60 deg */
#define C_PI_3     1.04719755119659774615
/* 3pi/8 rad or 67.5 deg */
#define C_3PI_8    1.17809724509617246442
/* pi/2 rad or 90 deg */
#define C_PI_2     1.57079632679489661923
/* 5pi/8 rad or 112.5 deg */
#define C_5PI_8    1.96349540849362077404
/* 2pi/3 rad or 120 deg */
#define C_2PI_3    2.09439510239319549231
/* 3pi/4 rad or 135 deg */
#define C_3PI_4    2.35619449019234492885
/* 5pi/6 rad or 150 deg */
#define C_5PI_6    2.61799387799149436539
/* 7pi/8 rad or 157.5 deg */
#define C_7PI_8    2.74889357189106908365
/* pi rad or 180 deg */
#define C_PI       3.14159265358979323846
/* 9pi/8 rad or 202.5 deg */
#define C_9PI_8    3.53429173528851739327
/* 7pi/6 rad or 210 deg */
#define C_7PI_6    3.66519142918809211154
/* 5pi/4 rad or 225 deg */
#define C_5PI_4    3.92699081698724154808
/* 4pi/3 rad or 240 deg */
#define C_4PI_3    4.18879020478639098462
/* 11pi/8 rad or 247.5 deg */
#define C_11PI_8   4.31968989868596570289
/* 3pi/2 rad or 270 deg */
#define C_3PI_2    4.71238898038468985769
/* 13pi/8 rad or 292.5 deg */
#define C_13PI_8   5.10508806208341401250
/* 5pi/3 rad or 300 deg */
#define C_5PI_3    5.23598775598298873077
/* 7pi/4 rad or 315 deg */
#define C_7PI_4    5.49778714378213816731
/* 11pi/6 rad or 330 deg */
#define C_11PI_6   5.75958653158128760385
/* 15pi/8 rad or 337.5 deg */
#define C_15PI_8   5.89048622548086232212
/* 2pi rad or 360 deg */
#define C_2PI      6.28318530717958647693

/* The value of sqrt(3)/2 */
#define C_SQRT3_2  0.86602540378443864676
/* The value of sqrt(2)/2 */
#define C_SQRT2_2  0.70710678118654752440

/* The value of cos(pi/8) or sqrt(2+sqrt(2))/2 */
#define C_MULT_1_8 0.92387953251128675613
/* The value of cos(pi/6) or sqrt(3)/2 */
#define C_MULT_1_6 0.86602540378443864676
/* The value of cos(pi/4) or sqrt(2)/2 */
#define C_MULT_1_4 0.70710678118654752440
/* The value of cos(pi/3) or 1/2 */
#define C_MULT_1_3 0.50000000000000000000
/* The value of cos(3pi/8) or sqrt(2-sqrt(2))/2 */
#define C_MULT_3_8 0.38268343236508977173

/* The conversion factor from degrees to radians (pi/180) */
#define U_PI_180   0.01745329251994329577
/* The conversion factor from radians to degrees (180/pi) */
#define U_180_PI   57.2957795130823208770

namespace btils {
    /** Acts as an assignment operator, but while returning the previous value of the thing being modified
     * @tparam Type A something that has a defined assignment operator (primitive type or otherwise)
     * @param lhs The left-hand side of an assignment operator (value will be altered)
     * @param newVal The right-hand side of an assignment operator (value will not be altered)
     * @returns The original value of lhs    */
    template <typename Type> Type set(Type &lhs, const Type &rhs) {
        const Type output = lhs;
        lhs = rhs;
        return output;
    }
    /** Acts as an addition assignment operator, but while returning the previous value of the thing being modified
     * @tparam Type A something that has a defined addition assignment operator (primitive type or otherwise)
     * @param lhs The left-hand side of an addition assignment operator (value will be altered)
     * @param newVal The right-hand side of an addition assignment operator (value will not be altered)
     * @returns The original value of lhs    */
    template <typename Type> Type adj(Type &lhs, const Type &rhs) {
        const Type output = lhs;
        lhs += rhs;
        return output;
    }

    /** Maps a value within a range onto a different range
     * @tparam InArithType An arithmetic data type for the input & starting range
     * @tparam OutArithType An arithmetic data type for the output & ending range
     * @param input Value to be mapped (of InArithType type)
     * @param inMin Minimum value for the starting range (of InArithType type)
     * @param inMax Maximum value for the starting range (of InArithType type)
     * @param outMin Minimum value for the ending range (of OutArithType type)
     * @param outMax Maximum value for the ending range (of OutArithType type)
     * @returns A value within the ending range matching the input value's position within the starting range (of OutArithType type)    */
    template <typename InArithType, typename OutArithType> OutArithType map(const InArithType &input, const InArithType &inMin, const InArithType &inMax, const OutArithType &outMin, const OutArithType &outMax) {
        static_assert(std::is_arithmetic< InArithType>::value, "InArithType must be an arithmetic type");
        static_assert(std::is_arithmetic<OutArithType>::value, "OutArithType must be an arithmetic type");
        return outMin + ((OutArithType)input - (OutArithType)inMin) * (outMax - outMin) / ((OutArithType)inMax - (OutArithType)inMin);
    }
    /** Maps a value within a range onto a different range
     * @tparam ArithType An arithmetic data type for the input & starting range
     * @param input Value to be mapped
     * @param inMin Minimum value for the starting range
     * @param inMax Maximum value for the starting range
     * @param outMin Minimum value for the ending range
     * @param outMax Maximum value for the ending range
     * @returns A value within the ending range matching the input value's position within the starting range    */
    template <typename ArithType> ArithType map(const ArithType &input, const ArithType &inMin, const ArithType &inMax, const ArithType &outMin, const ArithType &outMax) {return map<ArithType, ArithType>(input, inMin, inMax, outMin, outMax);}

    /** Normalize a value to a range (assuming the range is a loop)
     * @tparam ArithType An arithmetic data type for the input & range
     * @param input The value to normalize
     * @param rangeMin The smallest value making up the range (inclusive)
     * @param rangeMax The largest value making up the range (exclusive)
     * @returns A value normalized in the given range    */
    template <typename ArithType> ArithType normalize(const ArithType &input, const ArithType &rangeMin, const ArithType &rangeMax) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        const ArithType width = rangeMax - rangeMin;
        const ArithType value = input - rangeMin;
        return (value - (std::floor(value / width) * width)) + rangeMin;
    }
    /** Normalize a value to a range with a minimum value of 0 (assuming the range is a loop)
     * @tparam ArithType An arithmetic data type for the input & range
     * @param input The value to normalize
     * @param max The largest value making up the range (exclusive)
     * @returns A value normalized in the given range    */
    template <typename ArithType> ArithType normalize(const ArithType &input, const ArithType &max) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        return input - std::floor(input / max) * max;
    }

    /** Convert an angle from degrees to radians
     * @param angle An angle in degrees
     * @returns An angle in radians    */
    double        degToRad(const double &angle)                                {return angle * U_PI_180;}
    /** Convert an angle from radians to degrees
     * @param angle An angle in radians
     * @returns An angle in degrees    */
    double        radToDeg(const double &angle)                                {return angle * U_180_PI;}
    /** Convert an angle from either degrees to radians or radians to degrees
     * @param angle An angle in either radians or degrees
     * @param toRadians Whether to convert to radians or degrees
     * @returns An angle measured with the specified unit    */
    double    convertAngle(const double &angle, const bool &toRadians = true)  {return toRadians ? degToRad(angle) : radToDeg(angle);}
    /** Normalize an angle (in degrees) to be in the range of [0, 360)
     * @param angle An angle in degrees
     * @returns An angle in degrees, but in the range of [0, 360)    */
    double normalizeDegree(const double &angle)                                {return normalize<double>(angle, 360);}
    /** Normalize an angle (in radians) to be in the range of [0, 2 * pi)
     * @param angle An angle in radians
     * @returns An angle in radians, but in the range of [0, 2 * pi)    */
    double normalizeRadian(const double &angle)                                {return normalize<double>(angle, C_2PI);}
    /** Normalize an angle to be in the range of either [0, 2 * pi) or [0, 360)
     * @param angle An angle in either radians or degrees
     * @param useRadians Whether to normalize to [0, 2 * pi) for radians or [0, 360) for degrees
     * @returns An angle, but in the range of either [0, 2 * pi) or [0, 360)    */
    double  normalizeAngle(const double &angle, const bool &useRadians = true) {return useRadians ? normalizeRadian(angle) : normalizeDegree(angle);}

    class Percent {
        private:
            double Val = 0.0;

            static   bool UseAllowance;
            static double Allowance;

        public:
            Percent(const            Percent &input) : Val(input.getd()) {}
            Percent(const        signed char &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const      unsigned char &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const       signed short &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const     unsigned short &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const         signed int &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const       unsigned int &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const        signed long &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const      unsigned long &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const   signed long long &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const unsigned long long &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const              float &input) : Val(btils::normalize<double>((double)input, 1.0)) {}
            Percent(const             double &input) : Val(btils::normalize<double>(        input, 1.0)) {}
            Percent(const        long double &input) : Val(btils::normalize<double>((double)input, 1.0)) {}

            static double getAllowance() {return Allowance;}
            static double setAllowance(const double &allowance) {return btils::set<double>(Allowance, allowance);}
            static double adjAllowance(const double &   amount) {return btils::adj<double>(Allowance, amount);}

            Percent              getp() const {return *this;}
            signed char         getsc() const {return (       signed char)std::round(Val * 100.0);}
            unsigned char       getuc() const {return (     unsigned char)std::round(Val * 100.0);}
            signed short        getss() const {return (      signed short)std::round(Val * 100.0);}
            unsigned short      getus() const {return (    unsigned short)std::round(Val * 100.0);}
            signed int          getsi() const {return (        signed int)std::round(Val * 100.0);}
            unsigned int        getui() const {return (      unsigned int)std::round(Val * 100.0);}
            signed long         getsl() const {return (       signed long)std::round(Val * 100.0);}
            unsigned long       getul() const {return (     unsigned long)std::round(Val * 100.0);}
            signed long long   getsll() const {return (  signed long long)std::round(Val * 100.0);}
            unsigned long long getull() const {return (unsigned long long)std::round(Val * 100.0);}
            float                getf() const {return (             float)Val;}
            double               getd() const {return                     Val;}
            long double         getld() const {return (       long double)Val;}

            Percent             set(const            Percent &val) {return btils::set<Percent>(*this, val);}
            signed char         set(const        signed char &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            unsigned char       set(const      unsigned char &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            signed short        set(const       signed short &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            unsigned short      set(const     unsigned short &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            signed int          set(const         signed int &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            unsigned int        set(const       unsigned int &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            signed long         set(const        signed long &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            unsigned long       set(const      unsigned long &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            signed long long    set(const   signed long long &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            unsigned long long  set(const unsigned long long &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            float               set(const              float &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            double              set(const             double &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}
            long double         set(const        long double &val) {return btils::set<double>(Val, btils::normalize<double>(val, 1.0));}

            Percent             adj(const            Percent &val) {return set(Val +         val.getd());}
            signed char         adj(const        signed char &val) {return set(Val + (double)val);}
            unsigned char       adj(const      unsigned char &val) {return set(Val + (double)val);}
            signed short        adj(const       signed short &val) {return set(Val + (double)val);}
            unsigned short      adj(const     unsigned short &val) {return set(Val + (double)val);}
            signed int          adj(const         signed int &val) {return set(Val + (double)val);}
            unsigned int        adj(const       unsigned int &val) {return set(Val + (double)val);}
            signed long         adj(const        signed long &val) {return set(Val + (double)val);}
            unsigned long       adj(const      unsigned long &val) {return set(Val + (double)val);}
            signed long long    adj(const   signed long long &val) {return set(Val + (double)val);}
            unsigned long long  adj(const unsigned long long &val) {return set(Val + (double)val);}
            float               adj(const              float &val) {return set(Val + (double)val);}
            double              adj(const             double &val) {return set(Val +         val);}
            long double         adj(const        long double &val) {return set(Val + (double)val);}

            Percent  operator!()                       const {return Percent(1.0 - Val);}
            bool    operator==(const Percent &percent) const {return UseAllowance ? Val <= percent.getd() + Allowance && Val >= percent.getd() - Allowance : Val == percent.getd();}
            bool    operator!=(const Percent &percent) const {return UseAllowance ? Val >  percent.getd() + Allowance || Val <  percent.getd() - Allowance : Val != percent.getd();}
            bool     operator<(const Percent &percent) const {return Val <  percent.getd();}
            bool     operator>(const Percent &percent) const {return Val >  percent.getd();}
            bool    operator<=(const Percent &percent) const {return Val <= percent.getd();}
            bool    operator>=(const Percent &percent) const {return Val >= percent.getd();}

            Percent& operator+=(const            Percent &input) {set(Val +         input.getd());    return *this;}
            Percent& operator+=(const        signed char &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const      unsigned char &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const       signed short &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const     unsigned short &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const         signed int &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const       unsigned int &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const        signed long &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const      unsigned long &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const   signed long long &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const unsigned long long &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const              float &input) {set(Val + (double)input);           return *this;}
            Percent& operator+=(const             double &input) {set(Val +         input);           return *this;}
            Percent& operator+=(const        long double &input) {set(Val + (double)input);           return *this;}

            Percent& operator+=(const            Percent &input) {set(Val -         input.getd());    return *this;}
            Percent& operator+=(const        signed char &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const      unsigned char &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const       signed short &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const     unsigned short &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const         signed int &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const       unsigned int &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const        signed long &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const      unsigned long &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const   signed long long &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const unsigned long long &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const              float &input) {set(Val - (double)input);           return *this;}
            Percent& operator+=(const             double &input) {set(Val -         input);           return *this;}
            Percent& operator+=(const        long double &input) {set(Val - (double)input);           return *this;}
    };
    double Percent::Allowance = 0.00001;
    bool Percent::UseAllowance = true;
}

#endif /* UTILITIES */
