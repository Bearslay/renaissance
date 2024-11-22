#ifndef BENGINE_COORDINATE_2D_hpp
#define BENGINE_COORDINATE_2D_hpp

#include <cmath>
#include <vector>
#include <string>

namespace bengine {
    class coordinate_2d {
        public:
            enum class relation : unsigned char {
                DEFAULT = 0,
                EUCLIDEAN = 1,
                TAXICAB = 2,
                X_COMPONENT = 3,
                Y_COMPONENT = 4
            };

        private:
            double x_pos = 0;
            double y_pos = 0;

            static bengine::coordinate_2d::relation relation_metric;
            static bengine::coordinate_2d reference_point;

        public:
            coordinate_2d() {}
            coordinate_2d(const double &x_pos, const double &y_pos) {
                this->x_pos = x_pos;
                this->y_pos = y_pos;
            }
            coordinate_2d(const bengine::coordinate_2d &coord) {
                *this = coord;
            }

            static bengine::coordinate_2d::relation get_relation_metric() {
                return bengine::coordinate_2d::relation_metric;
            }
            static void set_relation_metric(const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation::DEFAULT) {
                bengine::coordinate_2d::relation_metric = (static_cast<unsigned char>(metric) > 4 || static_cast<unsigned char>(metric) < 0) ? bengine::coordinate_2d::relation::DEFAULT : metric;
            }
            static bengine::coordinate_2d get_reference_point() {
                return bengine::coordinate_2d::reference_point;
            }
            static void set_reference_point(const bengine::coordinate_2d &coord = bengine::coordinate_2d(0, 0)) {
                bengine::coordinate_2d::reference_point = coord;
            }
            static void set_reference_point(const double &x_pos, const double &y_pos) {
                bengine::coordinate_2d::set_reference_point(bengine::coordinate_2d(x_pos, y_pos));
            }

            double get_x_pos() const {
                return this->x_pos;
            }
            double get_y_pos() const {
                return this->y_pos;
            }
            void set_x_pos(const double &x_pos) {
                this->x_pos = x_pos;
            }
            void set_y_pos(const double &y_pos) {
                this->y_pos = y_pos;
            }
            void adjust_x_pos(const double &x_component) {
                this->x_pos += x_component;
            }
            void adjust_y_pos(const double &y_component) {
                this->y_pos += y_component;
            }

            void invert_x_and_y() {
                const double temp = this->x_pos;
                this->x_pos = this->y_pos;
                this->y_pos = temp;
            }
            void reflect_over_x_axis() {
                this->y_pos = -this->y_pos;
            }
            void reflect_over_y_axis() {
                this->x_pos = -this->x_pos;
            }
            void reflect_over_horizontal_line(const double &y_intercept) {
                this->y_pos = 2 * y_intercept - this->y_pos;
            }
            void reflect_over_vertical_line(const double &x_intercept) {
                this->x_pos = 2 * x_intercept - this->x_pos;
            }
            /** Reflect the coordinate over a line defined by y = mx + b (slope-intercept form)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y-intercept
             * 
             * Should the slope be undefined (NAN), nothing will happen as there is no x-intercept to form a vertical line from
             * 
             * \param slope The slope (m) of the line to reflect over
             * \param y_intercept The y-intercept (b) of the line to reflect over
             */
            void reflect_over_line(const double &slope, const double &y_intercept) {
                if (slope == 0) {
                    this->reflect_over_horizontal_line(y_intercept);
                    return;
                }
                if (std::isnan(slope)) {
                    return;
                }
                const double x_line_interception = (this->y_pos + this->x_pos / slope - y_intercept) / (slope + 1 / slope);
                this->x_pos = 2 * x_line_interception - this->x_pos;
                this->y_pos = 2 * (slope * x_line_interception + y_intercept) - this->y_pos;
            }
            /** Reflect the coordinate over a line defined by y - y1 = m(x - x1) (point-slope form)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y1
             * 
             * Should the slope be undefined (NAN), the coordinate will be reflected over the vertical line where x = x1
             * 
             * \param x_pos The x-position (x1) of the point defining the line to reflect over
             * \param y_pos The y-position (y1) of the point defining the line to reflect over
             * \param slope The slope (m) of the line to reflect over
             */
            void reflect_over_line(const double &x_pos, const double &y_pos, const double &slope) {
                if (std::isnan(slope)) {
                    this->reflect_over_vertical_line(x_pos);
                    return;
                }
                this->reflect_over_line(slope, y_pos - slope * x_pos);
            }
            /** Reflect the coordinate over a line defined by two inputted points (x1, y1) and (x2, y2)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y1 = y2
             * 
             * Should the slope be undefined (NAN), the coordinate will be reflected over the vertical line where x = x1 = x2
             * 
             * \param x_pos_1 The x-position (x1) of the first point defining the line to reflect over
             * \param y_pos_1 The y-position (y1) of the first point defining the line to reflect over
             * \param x_pos_2 The x-position (x2) of the second point defining the line to reflect over
             * \param y_pos_2 The y-position (y2) of the second point defining the line to reflect over
             */
            void reflect_over_line(const double &x_pos_1, const double &y_pos_1, const double &x_pos_2, const double &y_pos_2) {
                if (x_pos_2 - x_pos_1 == 0) {
                    this->reflect_over_vertical_line(x_pos_1);
                    return;
                }
                this->reflect_over_line(x_pos_1, y_pos_1, (y_pos_2 - y_pos_1) / (x_pos_2 - x_pos_1));
            }

            void stretch_horizontally(const double &scalar) {
                this->x_pos *= scalar;
            }
            void stretch_vertically(const double &scalar) {
                this->y_pos *= scalar;
            }
            // TODO: Add a stretch along any line
            
            void rotate_about_pivot(const bengine::coordinate_2d &pivot, const double &angle) {
                const double sin = std::sin(angle);
                const double cos = std::cos(angle);
                this->x_pos = (this->x_pos - pivot.get_x_pos()) * cos - (this->y_pos - pivot.get_y_pos()) * sin + pivot.get_x_pos();
                this->y_pos = (this->x_pos - pivot.get_x_pos()) * sin + (this->y_pos - pivot.get_y_pos()) * cos + pivot.get_y_pos();
            }
            void rotate_about_reference_point(const double &angle) {
                this->rotate_about_pivot(bengine::coordinate_2d::reference_point, angle);
            }

            static void rotate_points_about_pivot(std::vector<bengine::coordinate_2d> &points, const bengine::coordinate_2d &pivot, const double &angle) {
                const double sin = std::sin(angle);
                const double cos = std::cos(angle);
                for (std::size_t i = 0; i < points.size(); i++) {
                    points[i].set_x_pos((points[i].get_x_pos() - pivot.get_x_pos()) * cos - (points[i].get_y_pos() - pivot.get_y_pos()) * sin + pivot.get_x_pos());
                    points[i].set_x_pos((points[i].get_x_pos() - pivot.get_x_pos()) * sin + (points[i].get_y_pos() - pivot.get_y_pos()) * cos + pivot.get_y_pos());
                }
            }
            static void rotate_points_about_reference_point(std::vector<bengine::coordinate_2d> &points, const double &angle) {
                return bengine::coordinate_2d::rotate_points_about_pivot(points, bengine::coordinate_2d::reference_point, angle);
            }

            bengine::coordinate_2d& operator=(const bengine::coordinate_2d &rhs) {
                this->set_x_pos(rhs.get_x_pos());
                this->set_y_pos(rhs.get_y_pos());
                return *this;
            }
            bengine::coordinate_2d operator!() const {
                return bengine::coordinate_2d(-this->x_pos, -this->y_pos);
            }
            std::string to_string() const {
                return "(" + std::to_string(this->x_pos) + ", " + std::to_string(this->y_pos) + ")";
            }
            std::vector<double> to_vector() const {
                return {this->x_pos, this->y_pos};
            }

            bool is_equal_to(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::coordinate_2d::relation::DEFAULT:
                        return this->x_pos == other.get_x_pos() && this->y_pos == other.get_y_pos();
                    case bengine::coordinate_2d::relation::EUCLIDEAN:
                        return get_euclidean_distance_to(bengine::coordinate_2d::reference_point) == other.get_euclidean_distance_to(bengine::coordinate_2d::reference_point);
                    case bengine::coordinate_2d::relation::TAXICAB:
                        return get_taxicab_distance_to(bengine::coordinate_2d::reference_point) == other.get_taxicab_distance_to(bengine::coordinate_2d::reference_point);
                    case bengine::coordinate_2d::relation::X_COMPONENT:
                        return this->x_pos == other.get_x_pos();
                    case bengine::coordinate_2d::relation::Y_COMPONENT:
                        return this->y_pos == other.get_y_pos();
                }
            }
            bool is_not_equal_to(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                return !this->is_equal_to(other, metric);
            }
            bool is_less_than(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::coordinate_2d::relation::DEFAULT:
                    case bengine::coordinate_2d::relation::EUCLIDEAN:
                        return get_euclidean_distance_to(bengine::coordinate_2d::reference_point) < other.get_euclidean_distance_to(bengine::coordinate_2d::reference_point);
                    case bengine::coordinate_2d::relation::TAXICAB:
                        return get_taxicab_distance_to(bengine::coordinate_2d::reference_point) < other.get_taxicab_distance_to(bengine::coordinate_2d::reference_point);
                    case bengine::coordinate_2d::relation::X_COMPONENT:
                        return this->x_pos < other.get_x_pos();
                    case bengine::coordinate_2d::relation::Y_COMPONENT:
                        return this->y_pos < other.get_y_pos();
                }
            }
            bool is_greater_than(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                return other.is_less_than(*this, metric);
            }
            bool is_less_than_or_equal_to(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                return !this->is_greater_than(other, metric);
            }
            bool is_greater_than_or_equal_to(const bengine::coordinate_2d &other, const bengine::coordinate_2d::relation &metric = bengine::coordinate_2d::relation_metric) const {
                return !this->is_less_than(other, metric);
            }

            bool operator==(const bengine::coordinate_2d &rhs) const {
                return this->is_equal_to(rhs, bengine::coordinate_2d::relation_metric);
            }
            bool operator!=(const bengine::coordinate_2d &rhs) const {
                return this->is_not_equal_to(rhs, bengine::coordinate_2d::relation_metric);
            }
            bool operator<(const bengine::coordinate_2d &rhs) const {
                return this->is_less_than(rhs, bengine::coordinate_2d::relation_metric);
            }
            bool operator>(const bengine::coordinate_2d &rhs) const {
                return this->is_greater_than(rhs, bengine::coordinate_2d::relation_metric);
            }
            bool operator<=(const bengine::coordinate_2d &rhs) const {
                return this->is_less_than_or_equal_to(rhs, bengine::coordinate_2d::relation_metric);
            }
            bool operator>=(const bengine::coordinate_2d &rhs) const {
                return this->is_greater_than_or_equal_to(rhs, bengine::coordinate_2d::relation_metric);
            }

            bengine::coordinate_2d& operator+=(const bengine::coordinate_2d &rhs) {
                this->x_pos += rhs.get_x_pos();
                this->y_pos += rhs.get_y_pos();
                return *this;
            }
            bengine::coordinate_2d& operator-=(const bengine::coordinate_2d &rhs) {
                this->x_pos -= rhs.get_x_pos();
                this->y_pos -= rhs.get_y_pos();
                return *this;
            }
            bengine::coordinate_2d& operator*=(const double &rhs) {
                this->x_pos *= rhs;
                this->y_pos *= rhs;
                return *this;
            }
            bengine::coordinate_2d& operator/=(const double &rhs) {
                this->x_pos /= rhs;
                this->y_pos /= rhs;
                return *this;
            }
            bengine::coordinate_2d& operator*=(const double &rhs) {
                this->x_pos = std::fmod(this->x_pos, rhs);
                this->y_pos = std::fmod(this->y_pos, rhs);
                return *this;
            }
            bengine::coordinate_2d operator+(const bengine::coordinate_2d &rhs) const {
                return bengine::coordinate_2d(this->x_pos + rhs.get_x_pos(), this->y_pos + rhs.get_y_pos());
            }
            bengine::coordinate_2d operator-(const bengine::coordinate_2d &rhs) const {
                return bengine::coordinate_2d(this->x_pos - rhs.get_x_pos(), this->y_pos - rhs.get_y_pos());
            }
            bengine::coordinate_2d operator*(const double &rhs) const {
                return bengine::coordinate_2d(this->x_pos * rhs, this->x_pos * rhs);
            }
            bengine::coordinate_2d operator/(const double &rhs) const {
                return bengine::coordinate_2d(this->x_pos / rhs, this->y_pos / rhs);
            }
            bengine::coordinate_2d operator%(const double &rhs) const {
                return bengine::coordinate_2d(std::fmod(this->x_pos, rhs), std::fmod(this->y_pos, rhs));
            }

            double get_euclidean_distance_to(const bengine::coordinate_2d &other = bengine::coordinate_2d::reference_point) const {
                return std::sqrt((this->x_pos - other.get_x_pos()) * (this->x_pos - other.get_x_pos()) + (this->y_pos - other.get_y_pos()) * (this->y_pos - other.get_y_pos()));
            }
            double get_taxicab_distance_to(const bengine::coordinate_2d &other = bengine::coordinate_2d::reference_point) const {
                return std::fabs(this->x_pos - other.get_x_pos()) + std::fabs(this->y_pos - other.get_y_pos());\
            }
    };
    bengine::coordinate_2d::relation bengine::coordinate_2d::relation_metric = bengine::coordinate_2d::relation::DEFAULT;
    bengine::coordinate_2d bengine::coordinate_2d::reference_point = bengine::coordinate_2d(0, 0);
}

#endif // BENGINE_COORDINATE_2D_hpp
