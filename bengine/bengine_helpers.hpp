#ifndef BENGINE_HELPERS_hpp
#define BENGINE_HELPERS_hpp

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

namespace bengine {
    // \brief A class to contain dimensional data relating to a grid of cells within a set zone; primarily used to calculate and store the size of said cells
    class padded_grid {
        public:
            enum class alignments : const unsigned char {
                TOP_LEFT = 0,
                TOP_CENTER = 1,
                TOP_RIGHT = 2,
                CENTER_LEFT = 3,
                CENTER_CENTER = 4,
                CENTER_RIGHT = 5,
                BOTTOM_LEFT = 6,
                BOTTOM_CENTER = 7,
                BOTTOM_RIGHT = 8
            };

        private:
            // \brief The left-right and top-bottom alignment of the grid within the given zone; relates the top-left corner of the zone to the top-left corner of the top-left cell
            unsigned char alignment = static_cast<unsigned char>(alignments::CENTER_CENTER);
            // \brief Whether the cells are square or not
            bool square_cells = false;

            // \brief The width of the zone that the grid inhabits
            unsigned int width = 0;
            // \brief The height of the zone that the grid inhabits
            unsigned int height = 0;
            // \brief The amount of columns that the grid has
            unsigned short cols = 0;
            // \brief The amount of rows that the grid has
            unsigned short rows = 0;
            // \brief The horizontal distance between cells
            unsigned short gap_width = 0;
            // \brief The vertical distance between cells
            unsigned short gap_height = 0;

            // \brief The width of the area that the cells actually inhabit
            unsigned int usable_width = 0;
            // \brief The height of the area that the cells actually inhabit
            unsigned int usable_height = 0;
            // \brief The width of each cell
            unsigned int cell_width = 0;
            // \brief The height of each cell
            unsigned int cell_height = 0;
            // \brief The x-offset of the top-left cell's top-left corner from the zone's top-left corner
            unsigned short x_offset = 0;
            // \brief The y-offset of the top-left cell's top-left corner from the zone's top-left corner
            unsigned short y_offset = 0;

            // \brief Update all of the "output" values for the padded grid
            void update() {
                this->usable_width = this->width - this->gap_width * (this->cols - 1);
                this->usable_height = this->height - this->gap_height * (this->rows - 1);
                this->cell_width = this->usable_width / this->cols;
                this->cell_height = this->usable_height / this->rows;

                // When using square cells, both the cell's width and height are set to the smaller dimension so that the cells don't escape the zone
                if (this->square_cells) {
                    if (this->cell_width < this->cell_height) {
                        this->cell_height = this->cell_width;
                    } else {
                        this->cell_width = this->cell_height;
                    }
                }

                if (this->alignment % 3 == 0) {
                    this->x_offset = 0;
                } else if (this->alignment % 3 == 1) {
                    this->x_offset = (this->usable_width - this->cell_width * this->cols) / 2;
                } else {
                    this->x_offset = this->usable_width - this->cell_width * this->cols;
                }
                if (this->alignment <= 2) {
                    this->y_offset = 0;
                } else if (this->alignment <= 5) {
                    this->y_offset = (this->usable_height - this->cell_height * this->rows) / 2;
                } else {
                    this->y_offset = this->usable_width - this->cell_width * this->cols;
                }
            }

        public:
            /** bengine::padded_grid constructor
             * \param width The width of the zone that the grid will inhabit
             * \param height The height of the zone that the grid will inhabit
             * \param cols The amount of columns that the grid will have
             * \param rows The amount of rows that the grid will have
             * \param gap_width The amount of horiztonal space between cells
             * \param gap_height The amount of vertical space between cells
             * \param alignment The left-right and top-bottom alignment of the grid within the given zone; relates the top-left corner of the zone to the top-left corner of the top-left cell
             */
            padded_grid(const unsigned int &width, const unsigned int &height, const unsigned short &cols, const unsigned short &rows, const unsigned short &gap_width, const unsigned short &gap_height, const alignments &alignment = alignments::CENTER_CENTER) : width(width), height(height), cols(cols), rows(rows), gap_width(gap_width), gap_height(gap_height) {
                this->alignment = static_cast<unsigned char>(alignment) > static_cast<unsigned char>(alignments::BOTTOM_RIGHT) ? static_cast<unsigned char>(alignments::CENTER_CENTER) : static_cast<unsigned char>(alignment);
                this->update();
            }
            // \brief bengine::padded_grid deconstructor
            ~padded_grid() {}

            /** Get the alignment of the padded grid
             * \returns The alignment of the grid
             */
            unsigned char get_alignment() const {
                return this->alignment;
            }
            /** Set the alignmnet for the padded grid
             * \param alignment The new alignment for the padded grid
             */
            void set_alignment(const alignments &alignment) {
                this->alignment = static_cast<unsigned char>(alignment) > static_cast<unsigned char>(alignments::BOTTOM_RIGHT) ? static_cast<unsigned char>(alignments::CENTER_CENTER) : static_cast<unsigned char>(alignment);
                this->update();
            }

            /** Check if the grid uses square cells or not
             * \returns Whether the grid uses square cells or not
             */
            bool has_square_cells() const {
                return this->square_cells;
            }
            /** Set whether the grid should use square cells or not
             * \param state Whether the grid should use square cells or not
             */
            void set_cell_squareness(const bool &state = false) {
                this->square_cells = state;
                this->update();
            }
            // \brief Toggle whether the grid uses square cells or not
            void toggle_cell_squareness() {
                this->square_cells = !this->square_cells;
                this->update();
            }

            /** Get the width of the grid's zone
             * \returns The width of the grid's zone
             */
            unsigned int get_width() const {
                return this->width;
            }
            /** Get the height of the grid's zone
             * \returns The height of the grid's zone
             */
            unsigned int get_height() const {
                return this->height;
            }
            /** Get the amount of columns that the grid has
             * \returns The amount of columns that the grid has
             */
            unsigned short get_cols() const {
                return this->cols;
            }
            /** Get the amount of rows that the grid has
             * \returns The amount of rows that the grid has
             */
            unsigned short get_rows() const {
                return this->rows;
            }
            /** Get the horizontal distance between each cell
             * \returns The horizontal distance between each cell
             */
            unsigned short get_gap_width() const {
                return this->gap_width;
            }
            /** Get the vertical distance between each cell
             * \returns The vertical distance between each cell
             */
            unsigned short get_gap_height() const {
                return this->gap_height;
            }

            /** Set the width of the zone that the grid will inhabit
             * \param width The new width of the zone
             */
            void set_width(const unsigned int &width) {
                this->width = width;
                this->update();
            }
            /** Set the height of the zone that the grid will inhabit
             * \param width The new height of the zone
             */
            void set_height(const unsigned int &height) {
                this->height = height;
                this->update();
            }
            /** Set the amount of the columns that the grid will have
             * \param cols The new amount of columns
             */
            void set_cols(const unsigned short &cols) {
                this->cols = cols;
                this->update();
            }
            /** Set the amount of the rows that the grid will have
             * \param cols The new amount of rows
             */
            void set_rows(const unsigned short &rows) {
                this->rows = rows;
                this->update();
            }
            /** Set the horizontal distance between cells
             * \param gap_width The new horizontal distance between cells
             */
            void set_gap_width(const unsigned short &gap_width) {
                this->gap_width = gap_width;
                this->update();
            }
            /** Set the vertical distance between cells
             * \param gap_width The new vertical distance between cells
             */
            void set_gap_height(const unsigned short &gap_height) {
                this->gap_height = gap_height;
                this->update();
            }

            /** Get the width of the area that the cells inhabit
             * \returns The width of the area that the cells inhabit
             */
            unsigned int get_usable_width() const {
                return this->usable_width;
            }
            /** Get the height of the area that the cells inhabit
             * \returns The height of the area that the cells inhabit
             */
            unsigned int get_usable_height() const {
                return this->usable_height;
            }
            /** Get the width of each cell
             * \returns The width of each cell
             */
            unsigned int get_cell_width() const {
                return this->cell_width;
            }
            /** Get the height of each cell
             * \returns The height of each cell
             */
            unsigned int get_cell_height() const {
                return this->cell_height;
            }
            /** Get the horizontal distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             * \returns The horizontal distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             */
            unsigned short get_x_offset() const {
                return this->x_offset;
            }
            /** Get the vertical distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             * \returns The vertical distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             */
            unsigned short get_y_offset() const {
                return this->y_offset;
            }
    };

    // \brief A class containing useful functions designed for 4/8-bit autotiling
    class autotiler {
        private:
            // \brief Key containing the 47 bitmasks relevant to 8-bit autotiling
            const static unsigned char eight_bit_mask_key[47];

            // \brief List of unicode characters used in terminal-based 4-bit autotiling
            const static char* four_bit_unicode_key[32];
            // \brief List of unicode characters used in terminal-based 8-bit autotiling
            const static char* eight_bit_unicode_key[94];

            /** Calculate the 4-bit mask value for a given tile within a grid
             * 
             * Any bounds-checking needs to happen outside of this function
             * 
             * \param grid The grid containing 4-bit mask values
             * \param x The x-position (col) of the tile to update within the grid
             * \param y The y-position (row) of the tile to update within the grid
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The updated value of the indicated tile or -1 if the tile is is already -1
             */
            static char calculate_4_bit_mask(const std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &use_solid_boundaries = false) {
                // Check to see if the current tile would even display anything
                if (grid.at(y).at(x) < 0) {
                    return -1;
                }
                return (y > 0 ? (grid.at(y - 1).at(x) >= 0) : use_solid_boundaries) + (x > 0 ? (grid.at(y).at(x - 1) >= 0) : use_solid_boundaries) * 2 + (x < grid.at(0).size() - 1 ? (grid.at(y).at(x + 1) >= 0) : use_solid_boundaries) * 4 + (y < grid.size() - 1 ? (grid.at(y + 1).at(x) >= 0) : use_solid_boundaries) * 8;
            }

            /** Calculate the 8-bit mask value for a given tile within a grid
             * 
             * Any bounds-checking needs to happen outside of this function
             * 
             * \param grid The grid containing 8-bit mask values
             * \param x The x-position (col) of the tile to update within the grid
             * \param y The y-position (row) of the tile to update within the grid
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The updated value of the indicated tile or -1 if the tile is is already -1
             */
            static char calculate_8_bit_mask(const std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &use_solid_boundaries = false) {
                // Check to see if the current tile would even display anything
                if (grid.at(y).at(x) < 0) {
                    return -1;
                }

                const bool tl = y > 0 && x > 0 ? (grid.at(y - 1).at(x - 1) >= 0) : use_solid_boundaries;
                const bool t = y > 0 ? (grid.at(y - 1).at(x) >= 0) : use_solid_boundaries;
                const bool tr = y > 0 && x < grid.at(0).size() - 1 ? (grid.at(y - 1).at(x + 1) >= 0) : use_solid_boundaries;
                const bool l = x > 0 ? (grid.at(y).at(x - 1) >= 0) : use_solid_boundaries;
                const bool r = x < grid.at(0).size() - 1 ? (grid.at(y).at(x + 1) >= 0) : use_solid_boundaries;
                const bool bl = y < grid.size() - 1 && x > 0 ? (grid.at(y + 1).at(x - 1) >= 0) : use_solid_boundaries;
                const bool b = y < grid.size() - 1 ? (grid.at(y + 1).at(x) >= 0) : use_solid_boundaries;
                const bool br = y < grid.size() - 1 && x < grid.at(0).size() - 1 ? (grid.at(y + 1).at(x + 1) >= 0) : use_solid_boundaries;

                const unsigned char mask = (tl && t && l) + t * 2 + (tr && t && r) * 4 + l * 8 + r * 16 + (bl && b && l) * 32 + b * 64 + (br && b && r) * 128;
                for (unsigned char i = 0; i < 47; i++) {
                    if (mask == bengine::autotiler::eight_bit_mask_key[i]) {
                        return i;
                    }
                }
                return -1;
            }

        public:
            // \brief bengine::autotiler constructor
            autotiler() {}
            // \brief bengine::autotiler deconstructor
            ~autotiler() {}

            /** Change a tile and update surrounding ones in a 4-bit autotiling grid
             * \param grid Grid of indexing values that dictate the source frame for the texture sheet
             * \param x x-position of the changed tile in the grid
             * \param y y-position of the changed tile in the grid
             * \param state Whether to add or remove a tile in the indicated position
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The value of the updated tile
             */
            static char modify_4_bit_grid(std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &state = true, const bool &use_solid_boundaries = false) {
                if (y >= grid.size() || x >= grid.at(0).size()) {
                    return -1;
                }
                grid[y][x] = state - 1;

                for (char i = -1; i <= 1; i++) {
                    for (char j = -1; j <= 1; j++) {
                        // A 4-bit autotiling mask only requires cardinal directions, so the corners are skipped
                        if (std::abs(i) == 1 && std::abs(j) == 1) {
                            continue;
                        }
                        // Check if the current tile is in-bounds
                        if (y + i < 0 || y + i >= grid.size() || x + j < 0 || x + j >= grid.at(0).size()) {
                            continue;
                        }
                        // Update the mask value for the current tile
                        grid[y + i][x + j] = bengine::autotiler::calculate_4_bit_mask(grid, x + j, y + i, use_solid_boundaries);
                    }
                }
                return grid.at(y).at(x);
            }
            /** Change a tile and update surrounding ones in an 8-bit autotiling grid
             * \param grid Grid of indexing values that dictate the source frame for the texture sheet
             * \param x x-position of the changed tile in the grid
             * \param y y-position of the changed tile in the grid
             * \param state Whether to add or remove a tile in the indicated position
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The value of the updated tile
             */
            static char modify_8_bit_grid(std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &state = true, const bool &use_solid_boundaries = false) {
                if (y >= grid.size() || x >= grid.at(0).size()) {
                    return -1;
                }
                grid[y][x] = state - 1;

                for (char i = -1; i <= 1; i++) {
                    for (char j = -1; j <= 1; j++) {
                        // Check if the current tile is in-bounds
                        if (y + i < 0 || y + i >= grid.size() || x + j < 0 || x + j >= grid.at(0).size()) {
                            continue;
                        }
                        // Update the mask value for the current tile
                        grid[y + i][x + j] = bengine::autotiler::calculate_8_bit_mask(grid, x + j, y + i, use_solid_boundaries);
                    }
                }
                return grid.at(y).at(x);
            }

            /** Populate a grid of full/empty tiles with appropriate 4-bit mask values
             * \param grid The grid containing full (true) or empty (false) tiles to be populated
             * \param use_solid_boundaries Whether to consider the borders of the grid to have full or empty tiles
             * \returns A grid of the same dimensions as the input grid, but containing 4-bit mask values rather than boolean ones
             */
            static std::vector<std::vector<char>> populate_4_bit_grid(const std::vector<std::vector<bool>> &grid, const bool &use_solid_boundaries = false) {
                std::vector<std::vector<char>> output;
                for (std::size_t i = 0; i < grid.size(); i++) {
                    output.emplace_back();
                    for (std::size_t j = 0; j < grid.at(0).size(); j++) {
                        output[i].emplace_back(grid.at(i).at(j) ? 1 : -1);
                    }
                }
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        output[i][j] = bengine::autotiler::calculate_4_bit_mask(output, j, i, false);
                    }
                }
                return output;
            }

            /** Populate a grid of full/empty tiles with appropriate 8-bit mask values
             * \param grid The grid containing full (true) or empty (false) tiles to be populated
             * \param use_solid_boundaries Whether to consider the borders of the grid to have full or empty tiles
             * \returns A grid of the same dimensions as the input grid, but containing 8-bit mask values rather than boolean ones
             */
            static std::vector<std::vector<char>> populate_8_bit_grid(const std::vector<std::vector<bool>> &grid, const bool &use_solid_boundaries = false) {
                std::vector<std::vector<char>> output;
                for (std::size_t i = 0; i < grid.size(); i++) {
                    output.emplace_back();
                    for (std::size_t j = 0; j < grid.at(0).size(); j++) {
                        output[i].emplace_back(grid.at(i).at(j) ? 1 : -1);
                    }
                }
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        output[i][j] = bengine::autotiler::calculate_8_bit_mask(output, j, i, false);
                    }
                }
                return output;
            }

            /** Print a grid of 4-bit mask values to iostream using unicode block element characters
             * \param grid The grid of 4-bit mask values to print
             */
            static void print_4_bit_grid(const std::vector<std::vector<char>> &grid) {
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::four_bit_unicode_key[(unsigned char)grid.at(i).at(j)];
                    }
                    std::cout << "\n";
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::four_bit_unicode_key[(unsigned char)grid.at(i).at(j) + 16];
                    }
                    std::cout << "\n";
                }
            }

            /** Print a grid of 8-bit mask values to iostream using unicode block element characters
             * \param grid The grid of 8-bit mask values to print
             */
            static void print_8_bit_grid(const std::vector<std::vector<char>> &grid) {
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::eight_bit_unicode_key[(unsigned char)grid.at(i).at(j)];
                    }
                    std::cout << "\n";
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::eight_bit_unicode_key[(unsigned char)grid.at(i).at(j) + 47];
                    }
                    std::cout << "\n";
                }
            }
    };
    // \brief Key containing the 47 bitmasks relevant to 8-bit autotiling
    const unsigned char bengine::autotiler::eight_bit_mask_key[47] = {0, 2, 8, 10, 11, 16, 18, 22, 24, 26, 27, 30, 31, 64, 66, 72, 74, 75, 80, 82, 86, 88, 90, 91, 94, 95, 104, 106, 107, 120, 122, 123, 126, 127, 208, 210, 214, 216, 218, 219, 222, 223, 248, 250, 251, 254, 255};
    // \brief List of unicode characters used in terminal-based 4-bit autotiling
    const char* bengine::autotiler::four_bit_unicode_key[32] = {" ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", " ▄▄▄", " ██▄", "▄▄▄▄", "▄██▄", " ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", " ▄▄▄", " ██▄", "▄▄▄▄", "▄██▄", " ▀▀ ", " ▀▀ ", "▀▀▀ ", "▀▀▀ ", " ▀▀▀", " ▀▀▀", "▀▀▀▀", "▀▀▀▀", " ██ ", " ██ ", "▀██ ", "▀██ ", " ██▀", " ██▀", "▀██▀", "▀██▀"};
    // \brief List of unicode characters used in terminal-based 8-bit autotiling
    const char* bengine::autotiler::eight_bit_unicode_key[94] = {" ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", "███ ", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", "███ ", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", "▄▄▄ ", "▄██ ", "███ ", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▀▀ ", " ▀▀ ", "▀▀▀ ", "▀▀▀ ", "▀▀▀ ", " ▀▀▀", " ▀▀▀", " ▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", " ██ ", " ██ ", "▀██ ", "▀██ ", "▀██ ", " ██▀", " ██▀", " ██▀", "▀██▀", "▀██▀", "▀██▀", "▀██▀", "▀██▀", "███ ", "███ ", "███ ", "███▀", "███▀", "███▀", "███▀", "███▀", " ███", " ███", " ███", "▀███", "▀███", "▀███", "▀███", "▀███", "████", "████", "████", "████", "████"};

    // \brief A class containing useful functions that help with using bitwise operators in clever ways, especially in the realm of storing several values within a single integral variable
    class bitwise_manipulator {
        public:
            // \brief bengine::bitwise_manipulator constructor
            bitwise_manipulator() {}
            // \brief bengine::bitwise_manipulator deconstructor
            ~bitwise_manipulator() {}

            /** Set the indicated bits of an integral variable to 1 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bits The bits to activate, best inputted as 0x101 (5 as represented by a 3-bit integer) or something similar to easily visualize the bits that will be activated
             * \returns The value of the input but with the indicated bits set to 1 (literally input | bits)
             */
            template <class type> static type activate_bits(const type &input, const type &bits) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return input | bits;
            }
            /** Set the indicated bit of an integral variable to a 1 regardless of its original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bit The bit to activate where 0 indicates the least significant bit while 7 would be the most significant bit of an 8-bit type (like char); if this value is greater than the amount of bits available nothing will be changed
             * \returns The value of the input but with the indicated bit set to 1
             */
            template <class type> static type activate_bit(const type &input, const unsigned char &bit) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return input | static_cast<type>(std::pow<type, type>(2, bit));
            }
            /** Set the indicated bits of an integral variable to 0 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bits The bits to deactivate, best inputted as 0x101 (5 as represented by a 3-bit integer) or something similar to easily visualize the bits that will be deactivated
             * \returns The value of the input but with the indicated bits set to 0 (essentially input & (bits ^ type_max_value) )
             */
            template <class type> static type deactivate_bits(const type &input, const type &bits) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return input & (bits ^ static_cast<type>(std::pow<type, type>(2, sizeof(input) * 8) - 1));    // sizeof(input) * 8 yields amount of bits in input data type, which is then used to find max unsigned value of input data type with 2^bits - 1
            }
            /** Set the indicated bit of an integral variable to a 0 regardless of its original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bit The bit to activate where 0 indicates the least significant bit while 7 would be the most significant bit of an 8-bit type (like char); if this value is greater than the amount of bits available nothing will be changed
             * \returns The value of the input but with the indicated bit set to 0
             */
            template <class type> static type deactivate_bit(const type &input, const unsigned char &bit) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return bengine::bitwise_manipulator::deactivate_bits<type>(input, bengine::bitwise_manipulator::activate_bit<type>(0, bit));
            }
    };

    class kinematics_helper {
        private:
            // \brief The gravitational constant mainly used when looking at how the vertical motion of a projectile acts
            static double gravitational_constant;
        
        public:
            static double get_gravitational_constant() {
                return bengine::kinematics_helper::gravitational_constant;
            }
            static void set_gravitational_constant(const double &constant) {
                bengine::kinematics_helper::gravitational_constant = constant;
            }

            /** Calculate how long a projecticle will remain airborne
             * 
             */
            static double air_time(const double &magnitude, const double &angle, const double &y_difference) {
                const double y_velocity = magnitude * std::sin(angle);
                const double root = std::sqrt(y_velocity * y_velocity - 2 * bengine::kinematics_helper::gravitational_constant * y_difference);    // based off of a portion of the quadratic formula sqrt(b^2 - 4*a*c); b = vertical velocity, a = gravitational constant / 2, c = difference between y2 & y1 (dy)
                return std::fmax(root - y_velocity, -root - y_velocity) / -bengine::kinematics_helper::gravitational_constant;                     // return the larger solution to the quadratic formula b/c the smaller one will always be negative
            }

            static double peak_time(const double &magnitude, const double &angle) {
                return magnitude * std::sin(angle) / bengine::kinematics_helper::gravitational_constant;    // based off of x = -b/(2*a); x = time, b = vertical velocity, a = gravitational constant
            }

            static double peak_height(const double &magnitude, const double &angle, const double &starting_height) {
                const double peak = bengine::kinematics_helper::peak_time(magnitude, angle);
                return starting_height + magnitude * std::sin(angle) * peak - 0.5 * bengine::kinematics_helper::gravitational_constant * peak * peak;    // max height is achieved by substituting the time at which the projectile reaches its peak into the kinematic equation y = y0 + vy*t - 0.5*g*t^2
            }

            static double launch_angle(const double &magnitude, const double &x_difference, const double &y_difference, const bool &minimize_peak = true) {
                const double root = std::sqrt(magnitude * magnitude * magnitude * magnitude - bengine::kinematics_helper::gravitational_constant * (bengine::kinematics_helper::gravitational_constant * x_difference * x_difference + 2 * y_difference * magnitude * magnitude));
                const double angle_1 = std::atan2(magnitude * magnitude + root, bengine::kinematics_helper::gravitational_constant * x_difference);
                const double angle_2 = std::atan2(magnitude * magnitude - root, bengine::kinematics_helper::gravitational_constant * x_difference);
                return minimize_peak ? std::fmin(bengine::kinematics_helper::peak_height(magnitude, angle_1, 0), bengine::kinematics_helper::peak_height(magnitude, angle_2, 0)) : std::fmax(bengine::kinematics_helper::peak_height(magnitude, angle_1, 0), bengine::kinematics_helper::peak_height(magnitude, angle_2, 0));    // the peak heights of each angle are compared to determine which angle should be returned
            }

            static std::pair<double, double> landing_vector(const double &magnitude, const double &angle, const double &y_difference) {
                return {magnitude * std::cos(angle), magnitude * std::sin(angle) - bengine::kinematics_helper::gravitational_constant * bengine::kinematics_helper::air_time(magnitude, angle, y_difference)};    // the y-component of this vector is computed using the kinematic equation v = v0 + y*t; v = landing vertical velocity, v0 = initial vertical velocity, y = difference between y2 & y1 (dy), t = time for projectile to land
            }
    };
    double bengine::kinematics_helper::gravitational_constant = 9.0665;
}

#endif // BENGINE_HELPERS_hpp
