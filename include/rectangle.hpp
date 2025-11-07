#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "polygon.hpp"

namespace lab04 {

template <Scalar T>
class Rectangle : public PolygonFigure<Rectangle<T>, T, 4> {
    using base_type = PolygonFigure<Rectangle<T>, T, 4>;
    using point_type = typename base_type::point_type;

public:
    Rectangle() = default;

    Rectangle(const point_type& center, T width, T height) {
        if (width <= static_cast<T>(0) || height <= static_cast<T>(0)) {
            throw std::invalid_argument("rectangle sides must be positive");
        }
        using real = std::common_type_t<T, double>;
        const auto half_width = static_cast<real>(width) / static_cast<real>(2);
        const auto half_height = static_cast<real>(height) / static_cast<real>(2);
        const auto cx = static_cast<real>(center.x());
        const auto cy = static_cast<real>(center.y());
        std::array<point_type, 4> points{
            point_type{static_cast<T>(cx - half_width), static_cast<T>(cy - half_height)},
            point_type{static_cast<T>(cx + half_width), static_cast<T>(cy - half_height)},
            point_type{static_cast<T>(cx + half_width), static_cast<T>(cy + half_height)},
            point_type{static_cast<T>(cx - half_width), static_cast<T>(cy + half_height)}};
        this->assign(points);
    }

    Rectangle(const Rectangle&) = default;
    Rectangle& operator=(const Rectangle&) = default;
    Rectangle(Rectangle&&) noexcept = default;
    Rectangle& operator=(Rectangle&&) noexcept = default;
    ~Rectangle() override = default;

    [[nodiscard]] std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }

    [[nodiscard]] const char* shape_name() const { return "Rectangle"; }

    [[nodiscard]] double diagonal() const {
        const auto vertices = this->vertices();
        using real = std::common_type_t<T, double>;
        const auto dx = static_cast<real>(vertices[2].x()) - static_cast<real>(vertices[0].x());
        const auto dy = static_cast<real>(vertices[2].y()) - static_cast<real>(vertices[0].y());
        return std::sqrt(dx * dx + dy * dy);
    }

    [[nodiscard]] double circumscribed_circle_radius() const {
        return diagonal() / 2.0;
    }

private:
    [[nodiscard]] bool is_equal(const Figure<T>& other) const override {
        return base_type::is_equal(other);
    }
};

}  // namespace lab04

