#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "polygon.hpp"

namespace lab04 {

template <Scalar T>
class Square : public PolygonFigure<Square<T>, T, 4> {
    using base_type = PolygonFigure<Square<T>, T, 4>;
    using point_type = typename base_type::point_type;

public:
    Square() = default;

    Square(const point_type& center, T side) {
        if (side <= static_cast<T>(0)) {
            throw std::invalid_argument("square side must be positive");
        }
        using real = std::common_type_t<T, double>;
        const auto half_side = static_cast<real>(side) / static_cast<real>(2);
        const auto cx = static_cast<real>(center.x());
        const auto cy = static_cast<real>(center.y());
        std::array<point_type, 4> points{
            point_type{static_cast<T>(cx - half_side), static_cast<T>(cy - half_side)},
            point_type{static_cast<T>(cx + half_side), static_cast<T>(cy - half_side)},
            point_type{static_cast<T>(cx + half_side), static_cast<T>(cy + half_side)},
            point_type{static_cast<T>(cx - half_side), static_cast<T>(cy + half_side)}};
        this->assign(points);
    }

    Square(const Square&) = default;
    Square& operator=(const Square&) = default;
    Square(Square&&) noexcept = default;
    Square& operator=(Square&&) noexcept = default;
    ~Square() override = default;

    [[nodiscard]] std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Square>(*this);
    }

    [[nodiscard]] const char* shape_name() const { return "Square"; }

    [[nodiscard]] double inscribed_circle_radius() const {
        return side() / std::sqrt(2.0);
    }

    [[nodiscard]] double side() const {
        const auto vertices = this->vertices();
        using real = std::common_type_t<T, double>;
        const auto dx = static_cast<real>(vertices[1].x()) - static_cast<real>(vertices[0].x());
        const auto dy = static_cast<real>(vertices[1].y()) - static_cast<real>(vertices[0].y());
        return std::sqrt(dx * dx + dy * dy);
    }

private:
    [[nodiscard]] bool is_equal(const Figure<T>& other) const override {
        return base_type::is_equal(other);
    }
};

}  // namespace lab04
