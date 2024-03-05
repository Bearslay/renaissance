#ifndef ASTR
#define ASTR

#include <string>
#include <cmath>

/**
 * Contain custom toString()-esque and toWideString()-esque functions that mainly feature improved leading/trailing zero formatting
 */
namespace astr {
    /**
     * Round a floating-point value to the specified amount of decimal places
     * 
     * @tparam FloatType A floating-point data type (though using an integral one won't cause any hard; it just won't change anything)
     * @param input A floating-point value
     * @param decimals The amount of decimal places to round to (negative values can be used to round to whole number places; zero should round to a whole number)
     * @returns A floating-point value rounded from the inputted value
     */
    template <typename FloatType> FloatType round(FloatType input, int decimals = 2) {
        static_assert(std::is_arithmetic<FloatType>::value, "FloatType must be an arithmetic type");
        const FloatType multiplier = std::pow(10, decimals);
        return std::round((input) * multiplier) / multiplier;
    }

    /**
     * Get an arithmetic data type as a string
     * 
     * @tparam ArithType An arithmetic data type
     * @param input An arithmetic value
     * @param specifyPositive Whether to add positive signs or not when needed
     * @returns The inputted data type formatted as a string
     */
    template <typename ArithType> std::string toString(ArithType input, bool specifyPositive = false) {
        // std::to_string() only works for arithmetic data types
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

        std::string output = std::to_string(input);
        // Remove any extra decimal places on the end
        if (std::is_floating_point<ArithType>::value) {
            for (unsigned long i = output.length() - 1; i >= 0; i--) {
                if (output[i] == '.') {
                    output.erase(i, 1);
                    break;
                } else if (output[i] != '0') {
                    break;
                }
                output.erase(i, 1);
            }
        }
        return (specifyPositive && input > 0 ? "+" : "") + output;
    }
    /**
     * Get an arithmetic data type as a string
     * This particular overload allows for extra formatting with leading/trailing zeros
     * 
     * @tparam ArithType An arithmetic data type
     * @param input An arithmetic value
     * @param beforeDecimal The amount of digits to place before the decimal point
     * @param afterDecimal The amount of digits to place after the decimal point (even for integral types)
     * @param add If true, beforeDecimal/afterDecimal switch from amount of digits to amount of leading/trailing zeros (regardless of digits present already)
     * @param specifyPositive Whether to add positive signs or not when needed
     * @returns The inputted data type formatted as a string
     */
    template <typename ArithType> std::string toString_Places(ArithType input, unsigned long beforeDecimal, unsigned long afterDecimal = 0, bool add = false, bool specifyPositive = false) {
        std::string output = astr::toString(input, specifyPositive);
        unsigned long originalLength = output.length();

        if (beforeDecimal == 0 && afterDecimal == 0) {return output;}

        unsigned long beforeDecimalCount = 0;
        while (beforeDecimalCount < output.length()) {
            if (output[beforeDecimalCount] == '.') {break;}
            beforeDecimalCount++;
        }

        // Add leading zeros
        if (beforeDecimal > 0) {
            for (unsigned long i = add ? 0 : beforeDecimalCount; i < beforeDecimal; i++) {
                output.insert(input < 0 || specifyPositive ? 1 : 0, 1, '0');
            }

            if (afterDecimal == 0) {return output;}
        }

        unsigned long afterDecimalCount = output.find('.') == std::string::npos ? 0 : originalLength - beforeDecimalCount - 1;

        // Add trailing zeros
        if (afterDecimalCount == 0) {output += ".";}
        for (unsigned long i = add ? 0 : afterDecimalCount; i < afterDecimal; i++) {
            output += "0";
        }

        return output;
    }
    /**
     * Get an arithmetic data type as a string
     * This particular overload allows for extra formatting by specifying a length for each component
     * 
     * @tparam ArithType An arithmetic data type
     * @param input An arithmetic value
     * @param length The desired width of the output string
     * @param leading Whether to add the extra zeros to the beginning or end of the string
     * @param specifyPositive Whether to add positive signs or not when needed
     * @returns The inputted data type formatted as a string
     */
    template <typename ArithType> std::string toString_Length(ArithType input, unsigned long length, bool leading = true, bool specifyPositive = false) {
        std::string output = astr::toString(input, specifyPositive);
        bool hasDecimal = output.find('.') != std::string::npos;

        unsigned long beforeDecimal = 0;
        while (beforeDecimal < output.length()) {
            if (output[beforeDecimal] == '.') {break;}
            beforeDecimal++;
        }
        unsigned long afterDecimal = hasDecimal ? output.length() - beforeDecimal - 1 : 0;

        if (leading) {return astr::toString_Places(input, length - afterDecimal - (hasDecimal ? 1 : 0), 0, false, specifyPositive);}
        return astr::toString_Places(input, 0, length - beforeDecimal - 1, false, specifyPositive);
    }
    /**
     * Get an arithmetic data type as a string
     * This particular overload formats the string in scientific notion
     * 
     * @tparam ArithType An arithmetic data type
     * @param input An arithmetic value
     * @param decimals The amount of decimal places to have the input rounded to
     * @param exponentDigits The amount of digits the exponent will have, filling unused ones with zeros
     * @param e Whether to output the notion using '0.00e00' or using '00 x 10^00'
     * @param specifyPositive Whether to add positive signs or not when needed
     * @returns The inputted data type formatted as a string in scientific notion
     */
    template <typename ArithType> std::string toString_Sci(ArithType input, int decimals = 2, int exponentDigits = 1, bool e = true, bool specifyPositive = false) {
        static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");
        // If the input is integral, trying to get a decimal place won't work
        if (std::is_integral<ArithType>::value) {return astr::toString_Sci((double)input, decimals, exponentDigits, e, specifyPositive);}
        // If the input is already in the correct range, then we can save some computational work
        if (input < 10 && input > 0) {return astr::toString_Places(astr::round(input, decimals), 0, decimals, false, specifyPositive) + (e ? "e" : " x 10^") + astr::toString_Places(0, exponentDigits + (specifyPositive ? 1 : 0), 0, false, specifyPositive);}
        
        // Shift the input so that only one digit on on the left of the decimal point and get the correct exponent
        int exponent = 0;
        while (std::fabs(input) > 10) {
            input /= 10;
            exponent++;
        }
        while (std::fabs(input) < 1) {
            input *= 10;
            exponent--;
        }

        return astr::toString_Places(astr::round(input, decimals), 0, decimals, false, specifyPositive) + (e ? "e" : " x 10^") + astr::toString_Places(exponent, exponentDigits + (specifyPositive || exponent < 0 ? 1 : 0), 0, false, specifyPositive);
    }
}

#endif /* ASTR */
