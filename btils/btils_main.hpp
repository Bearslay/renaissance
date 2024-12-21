#ifndef BTILS_MAIN_hpp
#define BTILS_MAIN_hpp

#include <type_traits>
#include <cmath>
#include <vector>

// \brief pi/8 rad or 22.5 deg
#define C_PI_8      0.39269908169872415481
// \brief pi/6 rad or 30 deg
#define C_PI_6      0.52359877559829887308
// \brief pi/4 rad or 45 deg
#define C_PI_4      0.78539816339744830961
// \brief pi/3 rad or 60 deg
#define C_PI_3      1.04719755119659774615
// \brief 3pi/8 rad or 67.5 deg
#define C_3PI_8     1.17809724509617246442
// \brief pi/2 rad or 90 deg
#define C_PI_2      1.57079632679489661923
// \brief 5pi/8 rad or 112.5 deg
#define C_5PI_8     1.96349540849362077404
// \brief 2pi/3 rad or 120 deg
#define C_2PI_3     2.09439510239319549231
// \brief 3pi/4 rad or 135 deg
#define C_3PI_4     2.35619449019234492885
// \brief 5pi/6 rad or 150 deg
#define C_5PI_6     2.61799387799149436539
// \brief 7pi/8 rad or 157.5 deg
#define C_7PI_8     2.74889357189106908365
// \brief pi rad or 180 deg
#define C_PI        3.14159265358979323846
// \brief 9pi/8 rad or 202.5 deg
#define C_9PI_8     3.53429173528851739327
// \brief 7pi/6 rad or 210 deg
#define C_7PI_6     3.66519142918809211154
// \brief 5pi/4 rad or 225 deg
#define C_5PI_4     3.92699081698724154808
// \brief 4pi/3 rad or 240 deg
#define C_4PI_3     4.18879020478639098462
// \brief 11pi/8 rad or 247.5 deg
#define C_11PI_8    4.31968989868596570289
// \brief 3pi/2 rad or 270 deg
#define C_3PI_2     4.71238898038468985769
// \brief 13pi/8 rad or 292.5 deg
#define C_13PI_8    5.10508806208341401250
// \brief 5pi/3 rad or 300 deg
#define C_5PI_3     5.23598775598298873077
// \brief 7pi/4 rad or 315 deg
#define C_7PI_4     5.49778714378213816731
// \brief 11pi/6 rad or 330 deg
#define C_11PI_6    5.75958653158128760385
// \brief 15pi/8 rad or 337.5 deg
#define C_15PI_8    5.89048622548086232212
// \brief 2pi rad or 360 deg
#define C_2PI       6.28318530717958647693

// \brief The value of sqrt(3)/2
#define C_SQRT3_2   0.86602540378443864676
// \brief The value of sqrt(2)/2
#define C_SQRT2_2   0.70710678118654752440

// \brief The value of cos(pi/8) or sqrt(2+sqrt(2))/2
#define C_MULT_1_8  0.92387953251128675613
// \brief The value of cos(pi/6) or sqrt(3)/2
#define C_MULT_1_6  0.86602540378443864676
// \brief The value of cos(pi/4) or sqrt(2)/2
#define C_MULT_1_4  0.70710678118654752440
// \brief The value of cos(pi/3) or 1/2
#define C_MULT_1_3  0.50000000000000000000
// \brief The value of cos(3pi/8) or sqrt(2-sqrt(2))/2
#define C_MULT_3_8  0.38268343236508977173

// \brief The value of sqrt(2)
#define C_SQRT2     1.41421356237309504880
// \brief The value of sqrt(3)
#define C_SQRT3     1.73205080756887729353

// \brief The value of the number just to the left of 0
#define C_0_LEFT   -0.00000000000000000001
// \brief The value of the number just to the right of 0
#define C_0_RIGHT   0.00000000000000000001
// \brief The value of the number just to the left of pi
#define C_PI_LEFT   3.14159265358979323845
// \brief The value of the number just to the right of pi
#define C_PI_RIGHT  3.14159265358979323847
// \brief The value of the number just to the left of 2*pi
#define C_2PI_LEFT  6.28318530717958647692
// \brief The value of the number just to the right of 2*pi
#define C_2PI_RIGHT 6.28318530717958647694

// \brief The conversion factor from degrees to radians (pi/180)
#define U_PI_180   0.01745329251994329577
// \brief The conversion factor from radians to degrees (180/pi)
#define U_180_PI   57.2957795130823208770

namespace btils {
    /** Acts as an assignment operator, but while returning the previous value of the thing being modified
     * \tparam type A something that has a defined assignment operator (primitive type or otherwise)
     * \param lhs The left-hand side of an assignment operator (value will be altered)
     * \param rhs The right-hand side of an assignment operator (value will not be altered)
     * \returns The original value of lhs
     */
    template <class type> type set_and_retrieve(type &lhs, const type &rhs) {
        const type output = lhs;
        lhs = rhs;
        return output;
    }
    /** Acts as an addition assignment operator, but while returning the previous value of the thing being modified
     * \tparam type A something that has a defined addition and assignment operator (primitive type or otherwise)
     * \param lhs The left-hand side of an addition assignment operator (value will be altered)
     * \param rhs The right-hand side of an addition assignment operator (value will not be altered)
     * \returns The original value of lhs
     */
    template <class type> type adjust_and_retrieve(type &lhs, const type &rhs) {
        const type output = lhs;
        lhs = lhs + rhs;
        return output;
    }

    /** Maps a value within a range onto a different range to maintain relative value (with static type conversion built-in)
     * \tparam input_type An arithmetic data type for the input value amd range
     * \tparam output_type An arithmetic data type for the output value and range
     * \param input_value Value to be mapped from within the input range
     * \param input_range_minimum Minimum value for the starting range
     * \param input_range_maximum Maximum value for the starting range
     * \param output_range_minimum Minimum value for the ending range
     * \param output_range_maximum Maximum value for the ending range
     * \returns A value within the ending range matching the input value's position within the starting range
     */
    template <class input_type = double, class output_type = double> output_type map_value_to_range(const input_type &input_value, const input_type &input_range_minimum, const input_type &input_range_maximum, const output_type &output_range_minimum, const output_type &output_range_maximum) {
        static_assert(std::is_arithmetic<input_type>::value, "input_type must be an arithmetic type");
        static_assert(std::is_arithmetic<output_type>::value, "output_type must be an arithmetic type");
        return output_range_minimum + static_cast<output_type>((input_value - input_range_minimum) * (output_range_maximum - output_range_minimum) / (input_range_maximum - input_range_minimum));
    }
    /** Maps a value within a range onto a different range to maintain relative value
     * \tparam type An arithmetic data type for the input value amd range
     * \param input_value Value to be mapped from within the input range
     * \param input_range_minimum Minimum value for the starting range
     * \param input_range_maximum Maximum value for the starting range
     * \param output_range_minimum Minimum value for the ending range
     * \param output_range_maximum Maximum value for the ending range
     * \returns A value within the ending range matching the input value's position within the starting range
     */
    template <class type = double> type map_value_to_range(const type &input_value, const type &input_range_minimum, const type &input_range_maximum, const type &output_range_minimum, const type &output_range_maximum) {
        return btils::map_value_to_range<type, type>(input_value, input_range_minimum, input_range_maximum, output_range_minimum, output_range_maximum);
    }

    /** Normalize a value to a range to keep retain its value, but represent it within a desired range
     * \tparam type An arithmetic data type for the input and range
     * \param input_value The value to normalize
     * \param range_minimum The smallest value making up the range (inclusive)
     * \param range_maximum The largest value making up the range (exclusive)
     * \returns A value normalized in the given range
      */
    template <class type = double> type normalize_value_to_range(const type &input_value, const type &range_minimum, const type &range_maximum) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
        const type width = range_maximum - range_minimum;
        const type value = input_value - range_minimum;
        return (value - (std::floor(value / width) * width)) + range_minimum;
    }
    /** Normalize a value to a range to keep retain its value, but represent it within a desired range with a minimum value of 0
     * \tparam type An arithmetic data type for the input and range
     * \param input_value The value to normalize
     * \param maximum_value The largest value making up the range (exclusive)
     * \returns A value normalized in the given range
     */
    template <class type = double> type normalize_value_to_range(const type &input_value, const type &maximum_value) {
        return input_value - std::floor(input_value / maximum_value) * maximum_value;
    }

    /** Clamp a value to a range, either leaving it unchanged or as the minimum/maximum value of the range
     * \tparam type An arithmetic data type for the input and range
     * \param input_value The value to normalize
     * \param range_minimum The smallest value making up the range (inclusive)
     * \param range_maximum The largest value making up the range (inclusive)
     * \returns A value clamped to the given range
     */
    template <class type = double> type clamp_value_to_range(const type &input_value, const type &range_minimum, const type &range_maximum) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
        return input_value < range_minimum ? range_minimum : (input_value > range_maximum ? range_maximum : input_value);
    }
    /** Clamp a value to a range with a minimum value of 0
     * \tparam type An arithmetic data type for the input and range
     * \param input_value The value to normalize
     * \param maximum_value The largest value making up the range (inclusive)
     * \returns A value clamped to the given range (0 as a minimum value)
     */
    template <class type = double> type clamp_value_to_range(const type &input_value, const type &maximum_value) {
        return btils::clamp_value_to_range(input_value, 0, maximum_value);
    }

    /** Convert an angle from degrees to radians
     * \param angle An angle in degrees
     * \returns An angle in radians
     */
    double degrees_to_radians(const double &angle) {
        return angle * U_PI_180;
    }
    /** Convert an angle from radians to degrees
     * \param angle An angle in radians
     * \returns An angle in degrees
     */
    double radians_to_degrees(const double &angle) {
        return angle * U_180_PI;
    }
    /** Convert an angle from either degrees to radians or radians to degrees
     * \param angle An angle in either radians or degrees
     * \param convert_to_radians Whether to convert to radians or degrees
     * \returns An angle measured with the specified unit
     */
    double convert_angle(const double &angle, const bool &convert_to_radians = true) {
        return convert_to_radians ? btils::degrees_to_radians(angle) : btils::radians_to_degrees(angle);
    }

    /** Normalize an angle in degrees to be along the interval [0, 360)
     * \param angle An angle in degrees
     * \returns The same angle, but along the interval [0, 360)
     */
    double normalize_degree_angle(const double &angle) {
        return btils::normalize_value_to_range<double>(angle, 360.0);
    }
    /** Normalize an angle in radians to be along the interval [0, 2pi)
     * \param angle An angle in radians
     * \returns The same angle, but along the interval [0, 2pi)
     */
    double normalize_radian_angle(const double &angle) {
        return btils::normalize_value_to_range<double>(angle, C_2PI);
    }
    /** Normalize an angle to be in the range of either [0, 2 * pi) or [0, 360) depending on the unit inputted
     * \param angle An angle in either radians or degrees
     * \param use_radians Whether to use radian measures (true) or degree measures (false)
     * \returns The same angle, but along either the interval [0, 2pi) or [0, 360)
     */
    double normalize_angle(const double &angle, const bool &use_radians) {
        return use_radians ? btils::normalize_radian_angle(angle) : btils::normalize_degree_angle(angle);
    }

    /** Find the greatest common divisor/factor of two numbers
     * \tparam type An integral data type
     * \param num1 The first number to find the gcd of
     * \param num2 The second number to find the gcd of
     * \returns The greatest common divisor of the two inputted numbers (or 1 if either num1 or num2 are 0)
     */
    template <class type = int> type greatest_common_divisor(const type &num1, const type &num2) {
        static_assert(std::is_integral<type>::value, "type must be an integral type");
        if (num1 == 0 || num2 == 0) {
            return 1;
        }
        type a = num1, b = num2;

        while (a > 0 && b > 0) {
            if (a > b) {
                a %= b;
            } else {
                b %= a;
            }
        }
        return a == 0 ? b : a;
    }
    /** Find the least common multiple of two numbers
     * \tparam type An integral data type
     * \param num1 The first number to find the lcm of
     * \param num2 The second number to find the lcm of
     * \returns The least common multiple of the two inputted numbers
     */
    template <class type = int> type least_common_multiple(const type &num1, const type &num2) {
        static_assert(std::is_integral<type>::value, "type must be an integral type");
        return num1 / btils::greatest_common_divisor<type>(num1, num2) * num2;
    }
}

#endif // BTILS_MAIN_hpp
