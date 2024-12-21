#ifndef BTILS_MATRIX_hpp
#define BTILS_MATRIX_hpp

#include <vector>

namespace btils {
    /** Checks whether a 2D std::vector is rectangular or not
     * \tparam type Any datatype/class (not relevant for this function)
     * \param input The 2D std::vector to check
     * \returns Whether the input is rectangular (true) or jagged (false)
     */
    template <class type> bool is_rectangular(const std::vector<std::vector<type>> &input) {
        // If each sub-vector of the input vector is the same size as the first sub-vector, then the vector is rectangular
        for (std::size_t i = 1; i < input.size(); i++) {
            if (input.at(0).size() != input.at(i).size()) {
                return false;
            }
        }
        return true;
    }

    /** Rotate any rectangular 2D std::vector (matrix) 90 degrees
     * \tparam type Any datatype/class (not relevant for this function)
     * \param matrix The input vector to be rotated
     * \param rotate_ccw Whether to rotate counter-clockwise or not
     * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
     * \returns The rotated input vector (by 90 degrees)
     */
    template <class type> std::vector<std::vector<type>> rotate_matrix(const std::vector<std::vector<type>> &matrix, const bool &rotate_ccw, const bool &check_input = false) {
        if (check_input && btils::is_rectangular(matrix)) {
            return matrix;
        }
        std::vector<std::vector<type>> output;

        if (rotate_ccw) {
            for (std::size_t col = matrix.at(0).size(); col--;) {
                output.emplace_back();
                for (std::size_t row = 0; row < matrix.size(); row++) {
                    output[matrix.at(0).size() - 1 - col].emplace_back(matrix.at(row).at(col));
                }
            }
            return output;
        }
        for (std::size_t col = 0; col < matrix.at(0).size(); col++) {
            output.emplace_back();
            for (std::size_t row = matrix.size(); row--;) {
                output[col].emplace_back(matrix.at(row).at(col));
            }
        }
        return output;
    }

    /** Rotate any rectangular 2D std::vector (matrix) 90 degrees any amount of times
     * \tparam type Any datatype/class (not relevant for this function)
     * \param matrix The input vector to be rotated
     * \param rotations The amount of times to rotate the matrix; positive value for counter-clockwise and negative value for clockwise
     * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
     * \returns The rotated input vector (by 90 * rotations degrees)
     */
    template <class type> std::vector<std::vector<type>> rotate_matrix(const std::vector<std::vector<type>> &matrix, const int &rotations, const bool &check_input = false) {
        switch (rotations % 4) {
            default:
            case 0:
                return matrix;
            case 3:
            case -1:
                return btils::rotate_matrix<type>(matrix, false, check_input);
            case 1:
            case -3:
                return btils::rotate_matrix<type>(matrix, true, check_input);
            case 2:
            case -2:
                return btils::rotate_matrix<type>(btils::rotate_matrix<type>(matrix, true, check_input), true, check_input);
        }
        return matrix;
    }

    /** Flip any rectangular 2D std::vector (matrix)
     * \tparam type Any datatype/class (not relevant for this function)
     * \param matrix The inputted vector to be flipped
     * \param flip_vertically Whether to flip vertically (true) or horizontally (false)
     * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
     * \returns The flipped input vector
     */
    template <class type> std::vector<std::vector<type>> flip_matrix(const std::vector<std::vector<type>> &matrix, const bool &flip_vertically = true, const bool &check_input = false) {
        if (check_input && btils::is_rectangular(matrix)) {
            return matrix;
        }
        std::vector<std::vector<type>> output;

        if (flip_vertically) {
            for (std::size_t row = matrix.size(); row--;) {
                output.emplace_back();
                for (std::size_t col = 0; col < matrix.at(0).size(); col++) {
                    output[matrix.size() - 1 - row].emplace_back(matrix.at(row).at(col));
                }
            }
            return output;
        }
        for (std::size_t row = 0; row < matrix.size(); row++) {
            output.emplace_back();
            for (std::size_t col = matrix.at(0).size(); col--;) {
                output[row].emplace_back(matrix.at(row).at(col));
            }
        }
        return output;
    }

    /** Flip any rectangular 2D std::vector (matrix) any amount of times
     * \tparam type Any datatype/class (not relevant for this function)
     * \param matrix The inputted vector to be flipped
     * \param flips The amount of times to flip the matrixl; positive values to flip vertically and negative values to flip horizontally
     * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
     * \returns The flipped input vector
     */
    template <class type> std::vector<std::vector<type>> flip_matrix(const std::vector<std::vector<type>> &matrix, const int &flips, const bool &check_input = false) {
        switch (flips % 2) {
            default:
            case 0:
                return matrix;
            case 1:
                return btils::flip_matrix<type>(matrix, true, check_input);
            case -1:
                return btils::flip_matrix<type>(matrix, false, check_input);
        }
        return matrix;
    }
}

#endif // BTILS_MATRIX_hpp
