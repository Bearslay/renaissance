#ifndef BENGINE_COLLIDERS_hpp
#define BENGINE_COLLIDERS_hpp

#include <cmath>
#include <optional>

#include "btils.hpp"
#include "bengine_coordinate_2d.hpp"

namespace bengine {
    class basic_collider_2d {
        public:
            enum class fix_mode : const unsigned char {
                MOVE_SELF = 0,
                MOVE_OTHER = 1,
                MOVE_BOTH = 2
            };

        protected:
            double x_pos = 0;
            double y_pos = 0;
            double width = 0;
            double height = 0;

        public:
            basic_collider_2d() {}
            basic_collider_2d(const double &x, const double &y, const double &width, const double &height) {
                this->set_x_pos(x);
                this->set_y_pos(y);
                this->set_width(width);
                this->set_height(height);
            }
            basic_collider_2d(const bengine::basic_collider_2d &rhs) {
                *this = rhs;
            }
            ~basic_collider_2d() {}

            bengine::basic_collider_2d& operator=(const bengine::basic_collider_2d &rhs) {
                this->x_pos = rhs.get_x_pos();
                this->y_pos = rhs.get_y_pos();
                this->width = rhs.get_width();
                this->height = rhs.get_height();
                return *this;
            }

            std::string to_string() const {
                return "{" + btils::to_string<double>(this->x_pos) + ", " + btils::to_string<double>(this->y_pos) + ", " + btils::to_string<double>(this->width) + ", " + btils::to_string<double>(this->height) + "}";
            }

            double get_x_pos() const {
                return this->x_pos;
            }
            double get_y_pos() const {
                return this->y_pos;
            }
            double get_width() const {
                return this->width;
            }
            double get_height() const {
                return this->height;
            }

            double get_left_x() const {
                return this->get_x_pos();
            }
            double get_top_y() const {
                return this->get_y_pos();
            }
            double get_right_x() const {
                return this->x_pos + this->width;
            }
            double get_bottom_y() const {
                return this->y_pos + this->height;
            }

            void set_x_pos(const double &x_pos) {
                this->x_pos = x_pos;
            }
            void set_y_pos(const double &y_pos) {
                this->y_pos = y_pos;
            }
            void set_width(const double &width) {
                this->width = width < 0 ? -width : width;
            }
            void set_height(const double &height) {
                this->height = height < 0 ? -height : height;
            }

            void set_left_x(const double &x_pos) {
                this->set_x_pos(x_pos);
            }
            void set_top_y(const double &y_pos) {
                this->set_y_pos(y_pos);
            }
            void set_right_x(const double &x_pos) {
                this->set_width(x_pos - this->x_pos);
            }
            void set_bottom_y(const double &y_pos) {
                this->set_height(y_pos - this->y_pos);
            }

            bool detect_collision(const bengine::basic_collider_2d &other) const {
                return !(this->get_right_x() < other.get_left_x() || this->get_left_x() > other.get_right_x() || this->get_bottom_y() < other.get_top_y() || this->get_top_y() > other.get_bottom_y());
            }

            /** Fix the collision between two colliders so that they are no longer colliding
             * \param other The other bengine::basic_collider_2d that is being collided with
             * \param fix_mode The way that the collision should be fixed (mainly with the movement of either colliders)
             * \param do_initial_collision_check Whether to do a check for collision or not; this is recommended if no other check is performed prior to calling this function, but is optional otherwise
             * \returns False in the case that do_initial_collision_check is true but no collision was detected, true in all other cases
             */
            bool fix_collision(bengine::basic_collider_2d &other, const bengine::basic_collider_2d::fix_mode &fix_mode, const bool &do_initial_collision_check = true) {
                if (do_initial_collision_check) {
                    if (!this->detect_collision(other)) {
                        return false;
                    }
                }

                const double right_overlap = this->get_right_x() - other.get_left_x();
                const double top_overlap = this->get_bottom_y() - other.get_top_y();
                const double left_overlap = other.get_right_x() - this->get_left_x();
                const double bottom_overlap = other.get_bottom_y() - this->get_top_y();

                double smallest_overlap = right_overlap;
                double x_vector = -smallest_overlap, y_vector = 0;

                if (top_overlap < smallest_overlap) {
                    smallest_overlap = top_overlap;
                    x_vector = 0;
                    y_vector = -smallest_overlap;
                }
                if (left_overlap < smallest_overlap) {
                    smallest_overlap = left_overlap;
                    x_vector = smallest_overlap;
                    y_vector = 0;
                }
                if (bottom_overlap < smallest_overlap) {
                    smallest_overlap = bottom_overlap;
                    x_vector = 0;
                    y_vector = smallest_overlap;
                }

                switch (fix_mode) {
                    case bengine::basic_collider_2d::fix_mode::MOVE_SELF:
                        this->x_pos += x_vector;
                        this->y_pos += y_vector;
                        break;
                    case bengine::basic_collider_2d::fix_mode::MOVE_OTHER:
                        other.set_x_pos(other.get_x_pos() - x_vector);
                        other.set_y_pos(other.get_y_pos() - y_vector);
                        break;
                    case bengine::basic_collider_2d::fix_mode::MOVE_BOTH:
                        this->x_pos += x_vector / 2;
                        this->y_pos += y_vector / 2;
                        other.set_x_pos(other.get_x_pos() - x_vector / 2);
                        other.set_y_pos(other.get_y_pos() - y_vector / 2);
                        break;
                }
                return true;
            }
    };

    class hitscanner_2d {
        private:
            double x_pos = 0;
            double y_pos = 0;
            double angle = 0;
            double range = -1;

        public:
            hitscanner_2d() {}
            hitscanner_2d(const double &x_pos, const double &y_pos, const double &angle, const double &range) {
                this->x_pos = x_pos;
                this->y_pos = y_pos;
                this->angle = btils::normalize_radian_angle(angle);
                this->range = range < 0 ? -1 : range;
            }
            hitscanner_2d(const bengine::hitscanner_2d &rhs) {
                *this = rhs;
            }
            ~hitscanner_2d() {}

            bengine::hitscanner_2d& operator=(const bengine::hitscanner_2d &rhs) {
                this->x_pos = rhs.get_x_pos();
                this->y_pos = rhs.get_y_pos();
                this->angle = rhs.get_angle();
                this->range = rhs.get_range();
            }

            double get_x_pos() const {
                return this->x_pos;
            }
            double get_y_pos() const {
                return this->y_pos;
            }
            double get_angle() const {
                return this->angle;
            }
            double get_range() const {
                return this->range;
            }

            void set_x_pos(const double &x_pos) {
                this->x_pos = x_pos;
            }
            void set_y_pos(const double &y_pos) {
                this->y_pos = y_pos;
            }
            void set_angle(const double &angle) {
                this->angle = btils::normalize_radian_angle(angle);
            }
            void set_range(const double &range) {
                this->range = range < 0 ? -1 : range;
            }

            std::optional<bengine::coordinate_2d<double>> get_hit(const bengine::basic_collider_2d &collider) const {
                // If the hitscanner is inside the collider, then it should hit no matter what angle or range it has
                if (this->x_pos >= collider.get_left_x() && this->x_pos <= collider.get_right_x() && this->y_pos >= collider.get_top_y() && this->y_pos <= collider.get_bottom_y()) {
                    return bengine::coordinate_2d<double>(this->x_pos, this->y_pos);
                }
                // If the hitscanner is outside the collider and has a range of zero, then it will never hit the collider
                if (this->range == 0) {
                    return std::nullopt;
                }

                // Handle the four most common angles immediately and also handle any potential divide-by-zero errors that would occur with cos(PI/2) or cos(3*PI/2)
                // if (this->angle == 0) {
                //     return this->x_pos + this->range >= collider.get_left_x() && this->y_pos >= collider.get_top_y() && this->y_pos <= collider.get_bottom_y();
                // } else if (this->angle == C_PI_2) {
                //     return this->y_pos - this->range <= collider.get_bottom_y() && this->x_pos >= collider.get_left_x() && this->x_pos <= collider.get_right_x();
                // } else if (this->angle == C_PI) {
                //     return this->x_pos - this->range <= collider.get_right_x() && this->y_pos >= collider.get_top_y() && this->y_pos <= collider.get_bottom_y();
                // } else if (this->angle == C_3PI_2) {
                //     return this->y_pos + this->range >= collider.get_top_y() && this->x_pos >= collider.get_left_x() && this->x_pos <= collider.get_right_x();
                // }

                const double x_component = this->range * std::cos(this->angle);
                const double y_component = this->range * std::sin(this->angle);

                if (this->range > 0) {

                }

                const double slope = y_component / x_component;

                const double y_value = slope * (this->angle < C_PI_2 || this->angle > C_3PI_2 ? collider.get_left_x() : collider.get_right_x()) + this->y_pos;
                if (y_value >= collider.get_top_y() && y_value <= collider.get_bottom_y()) {
                    return bengine::coordinate_2d<double>((this->angle < C_PI_2 || this->angle > C_3PI_2 ? collider.get_left_x() : collider.get_right_x()), y_value);
                }
                const double x_value = ((this->angle > C_PI_2 && this->angle < C_3PI_2 ? collider.get_top_y() : collider.get_bottom_y()) - this->y_pos) / slope;
                if (x_value >= collider.get_left_x() && x_value <= collider.get_right_x()) {
                    return bengine::coordinate_2d<double>(x_value, (this->angle > C_PI_2 && this->angle < C_3PI_2 ? collider.get_top_y() : collider.get_bottom_y()));
                }
                return std::nullopt;
            }
    };
}

#endif // BENGINE_COLLIDERS_hpp
