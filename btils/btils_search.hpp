#ifndef BTILS_SEARCH_hpp
#define BTILS_SEARCH_hpp

#include <vector>
#include <optional>

namespace btils {
    /** Get the index of a target element within an unsorted/sorted list (note that it is recommended to use binary search for sorted lists)
     * \tparam Type Any datatype/class; must have equality operators available
     * \param list The list to search through
     * \param target The element to locate
     * \param lower_bounds The lowest index within the list to search from (can be used to search a particular section of a larger list)
     * \param upper_bounds the highest index within the list to search to (can be used to search a particular section of a larger list)
     * \returns An std::optional object that either contains the index of the target element if found, or will be an std::nullopt if the target is not present in the list between the inputted bounds
     */
    template <class type> std::optional<std::size_t> linear_search(const std::vector<type> &list, const type &target, const std::size_t &lower_bounds = 0, const std::size_t &upper_bounds = __UINT64_MAX__) {
        if (list.size() == 0) {
            return std::nullopt;
        }

        const std::size_t start = lower_bounds >= list.size() ? list.size() - 1 : lower_bounds;
        const std::size_t end = upper_bounds >= list.size() ? list.size() - 1 : upper_bounds;
        for (std::size_t index = start; index <= end; index++) {
            if (list.at(index) == target) {
                return index;
            }
        }
        return std::nullopt;
    }

    /** Get the index of a target element within a sorted list
     * \tparam Type Any datatype/class; must have relational and equality operators available
     * \param list The sorted list to search through
     * \param target The element to locate
     * \param lower_bounds The lowest index within the list to search from (can be used to search a particular section of a larger list)
     * \param upper_bounds the highest index within the list to search to (can be used to search a particular section of a larger list)
     * \returns An std::optional object that either contains the index of the target element if found, or will be an std::nullopt if the target is not present in the list between the inputted bounds
     */
    template <class type> std::optional<std::size_t> binary_search(const std::vector<type> &list, const type &target, const std::size_t &lower_bounds = 0, const std::size_t &upper_bounds = __UINT64_MAX__) {
        if (list.size() == 0) {
            return std::nullopt;
        }

        std::size_t start = lower_bounds > list.size() ? list.size() - 1 : lower_bounds;
        std::size_t end = upper_bounds > list.size() ? list.size() - 1 : upper_bounds;
        while (start <= end) {
            std::size_t index = start + (end - start) / 2;
            if (list.at(index) == target) {
                return index;
            }
            if (list.at(index) < target) {
                start = index + 1;
            } else {
                end = index - 1;
            }
        }
        return std::nullopt;
    }
}

#endif // BTILS_SEARCH_hpp
