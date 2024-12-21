#ifndef BTILS_STRING_hpp
#define BTILS_STRING_hpp

#include <type_traits>
#include <string>

namespace btils {
    /** Convert an arithmetic data type to an std::string; trailing zeros and decimals are omitted from floating-point conversions
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \returns An std::string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
     */
    template <class type> std::string to_string(const type &input) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");

        // std::to_string() is acceptable for anything other than floating-point types
        if (std::is_integral<type>::value) {
            return std::to_string(input);
        }

        std::string output = std::to_string(input);
        // Remove the last character if it is either a zero or a decimal
        while (output.back() == '0') {
            output.pop_back();
            // Break seperately at the decimal so that zeros in positive places aren't removed
            if (output.back() == '.') {
                output.pop_back();
                break;
            }
        }
        return output;
    }

    /** Convert an arithmetic data type to an std::string while having the ability to specify sign; trailing zeros and decimals are omitted from floating-point conversions
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \param always_include_sign Whether to include a positive sign if the input is positive or not (a negative sign will be included regardless of the value of this parameter)
     * \returns An std::string where each character represents a part of the input (and might also specifically include a sign); trailing zeros and decimals are omitted from floating-point conversions
     */
    template <class type> std::string to_string(const type &input, const bool always_include_sign) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
        return (always_include_sign && input >= 0 ? "+" : "") + btils::to_string<type>(input);
    }

    /** Convert an arithmetic data type to an std::string with extra leading/trailing zeros if specified
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \param left_digits The minimum amount of digits that will be to the left of the decimal (can be exceeded depending on input value); reached by adding leading zeros
     * \param right_digits The minimum amount of digits that will be to the right of the decimal (can be exceeded depending on input value); reached by adding trailing zeros
     * \returns An std::string where each character represents a part of the input; leading and trailing zeros are also included in order to meet certain requirements
     */
    template <class type> std::string to_string_with_added_zeros(const type &input, const unsigned long int &left_digits, const unsigned long int &right_digits) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
        std::string output = btils::to_string<type>(input);
        const unsigned long int decimal_index = output.find('.');
        const unsigned long int left_length = decimal_index == std::string::npos ? output.length() : decimal_index;
        const unsigned long int right_length = decimal_index == std::string::npos ? 0 : output.length() - left_length - 1;
        const bool insert_position = input < 0;

        // Add on the leading zeros
        for (unsigned long int i = left_length; i < left_digits; i++) {
            output.insert(insert_position, "0");
        }
        // Return early if no trailing zeros are needed
        if (right_digits == 0) {
            return output;
        }

        // Add a decimal place if it is missing
        if (decimal_index == std::string::npos) {
            output += ".";
        }
        // Add on the trailing zeros
        for (unsigned long int i = right_length; i < right_digits; i++) {
            output += '0';
        }
        return output;
    }

    /** Convert an arithmetic data type to an std::string with extra leading/trailing zeros if specified
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \param left_digits The minimum amount of digits that will be to the left of the decimal (can be exceeded depending on input value); reached by adding leading zeros
     * \param right_digits The minimum amount of digits that will be to the right of the decimal (can be exceeded depending on input value); reached by adding trailing zeros
     * \returns An std::string where each character represents a part of the input; leading and trailing zeros are also included in order to meet certain parameters
     */
    template <class type> std::string to_string_with_added_zeros(const type &input, const unsigned long int &left_digits, const unsigned long int &right_digits, const bool &always_include_sign) {
        return (always_include_sign && input >= 0 ? "+" : "") + btils::to_string_with_added_zeros<type>(input, left_digits, right_digits);
    }

    /** Convert an arithmetic data type to an std::string with a specified minimum length
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \param length The minimum length of the output std::string (reached using leading/trailing zeros)
     * \param add_leading_zeros Whether to add leading zeros or not (trailing zeros) in order to reach the minimum length
     * \returns An std::string where each character represents a part of the input and also beginning with either a positive or negatie sign; leading or trailing zeros are included to reach a specified minimum string length
     */
    template <class type> std::string to_string_with_target_length(const type &input, const unsigned long int &length, const bool &add_leading_zeros) {
        static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
        std::string output = btils::to_string<type>(input);

        // Adding leading zeros until the desired length is reached
        if (add_leading_zeros) {
            const bool insert_position = input < 0;
            while (output.length() < length) {
                output.insert(insert_position, "0");
            }
            return output;
        }
        
        if (output.find('.') == std::string::npos) {
            // If the output would exceed the desired length by adding on a '.0', then a leading zero is added instead
            if (output.length() >= length - 1) {
                output.insert(input < 0, "0");
                return output;
            }
            output += ".0";
        }

        // Add trailing zeros until the desired length is reached
        while (output.length() < length) {
            output += "0";
        }
        return output;
    }

    /** Convert an arithmetic data type to an std::string with a specified minimum length
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into a string
     * \param length The minimum length of the output std::string (reached using leading/trailing zeros)
     * \param add_leading_zeros Whether to add leading zeros (true) or add trailing zeros (false) to reach the minimum length
     * \param always_include_sign Whether to include a positive sign if the input is positive or not (sign contributes to the desired length)
     * \returns An std::string where each character represents a part of the input; leading or trailing zeros are included to reach a specified minimum string length
     */
    template <class type> std::string to_string_with_target_length(const type &input, const unsigned long int &length, const bool &add_leading_zeros, const bool &always_include_sign) {
        return (always_include_sign && input >= 0 ? "+" : "") + btils::to_string_with_target_length<type>(input, length, add_leading_zeros);
    }

    /** Convert an arithmetic data type to an std::u16string making use of the btils::to_string() function
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into an std::u16string
     * \returns An std::u16string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
     */
    template <class type> std::u16string to_u16string(const type &input) {
        const std::string str = btils::to_string<type>(input);
        return {str.begin(), str.end()};
    }
    /** Convert an arithmetic data type to an std::u32string making use of the btils::to_string() function
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into an std::u32string
     * \returns An std::u32string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
     */
    template <class type> std::u32string to_u32string(const type &input) {
        const std::string str = btils::to_string<type>(input);
        return {str.begin(), str.end()};
    }
    /** Convert an arithmetic data type to an std::wstring making use of the btils::to_string() function
     * \tparam type An arithmetic data type to convert from
     * \param input The arithmetic value to convert into an std::wstring
     * \returns An std::wstring where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
     */
    template <class type> std::wstring to_wstring(const type &input) {
        const std::string str = btils::to_string<type>(input);
        return {str.begin(), str.end()};
    }

    /** Convert an std::string to an std::u16string
     * \param input The std::string to convert from
     * \returns An std::u16string derived from the inputted std::string
     */
    template <> std::u16string to_u16string<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }
    /** Convert an std::string to an std::u32string
     * \param input The std::string to convert from
     * \returns An std::u32string derived from the inputted std::string
     */
    template <> std::u32string to_u32string<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }
    /** Convert an std::string to an std::wstring
     * \param input The std::string to convert from
     * \returns An std::wstring derived from the inputted std::string
     */
    template <> std::wstring to_wstring<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }
}

#endif // BTILS_STRING_hpp
