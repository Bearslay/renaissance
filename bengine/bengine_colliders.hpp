#ifndef BENGINE_COLLIDERS_hpp
#define BENGINE_COLLIDERS_hpp

#include <cmath>

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

            // (1, 1) -> (0, 0)
            // 0.5 x 0.5 -> 13 x 2

            bool fix_collision(bengine::basic_collider_2d &other, const fix_mode &fix_mode, const bool &do_initial_collision_check = true) {
                if (do_initial_collision_check) {
                    if (!this->detect_collision(other)) {
                        return false;
                    }
                }

                const double x_val = (this->x_pos + this->width / 2) - (other.get_x_pos() + other.get_width() / 2);
                const double y_val = (this->y_pos + this->height / 2) * (this->width / this->height) - (other.get_y_pos() + other.get_height() / 2) * (other.get_width() / other.get_height());

                if (std::fabs(x_val) >= std::fabs(y_val)) {
                    switch (fix_mode) {
                        case fix_mode::MOVE_SELF:
                            this->x_pos = x_val < 0 ? other.get_x_pos() - this->width : other.get_x_pos() + other.get_width();
                            break;
                        case fix_mode::MOVE_OTHER:
                            other.set_x_pos(x_val < 0 ? this->x_pos + this->width : this->x_pos - other.get_width());
                            break;
                        case fix_mode::MOVE_BOTH:
                            break;
                    }
                } else {
                    switch (fix_mode) {
                        case fix_mode::MOVE_SELF:
                            this->y_pos = y_val < 0 ? other.get_y_pos() - this->height : other.get_y_pos() + other.get_height();
                            break;
                        case fix_mode::MOVE_OTHER:
                            other.set_y_pos(y_val < 0 ? this->y_pos + this->height : this->y_pos - other.get_height());
                            break;
                        case fix_mode::MOVE_BOTH:
                            break;
                    }
                }
                return true;
            }
    };
}

#endif // BENGINE_COLLIDERS_hpp
