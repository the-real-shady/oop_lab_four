#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "polygon.hpp"

namespace lab04 {

template <Scalar T>
class Triangle : public PolygonFigure<Triangle<T>, T, 3> {
    using base_type = PolygonFigure<Triangle<T>, T, 3>;
    using point_type = typename base_type::point_type;

public:
    Triangle() = default;

    Triangle(const point_type& apex, const point_type& base_left, const point_type& base_right) {
        if (!is_isosceles(apex, base_left, base_right)) {
            throw std::invalid_argument("triangle must be isosceles");
        }
        this->assign({apex, base_left, base_right});
    }

    Triangle(const point_type& center, T base_width, T height) {
        if (base_width <= static_cast<T>(0) || height <= static_cast<T>(0)) {
            throw std::invalid_argument("triangle dimensions must be positive");
        }
        using real = std::common_type_t<T, double>;
        const auto half_base = static_cast<real>(base_width) / static_cast<real>(2);
        const auto h = static_cast<real>(height);
        const auto cx = static_cast<real>(center.x());
        const auto cy = static_cast<real>(center.y());

        const point_type apex{
            static_cast<T>(cx),
            static_cast<T>(cy + h * static_cast<real>(2) / static_cast<real>(3))};
        const point_type base_left{
            static_cast<T>(cx - half_base),
            static_cast<T>(cy - h / static_cast<real>(3))};
        const point_type base_right{
            static_cast<T>(cx + half_base),
            static_cast<T>(cy - h / static_cast<real>(3))};

        this->assign({apex, base_left, base_right});
    }

    Triangle(const Triangle&) = default;
    Triangle& operator=(const Triangle&) = default;
    Triangle(Triangle&&) noexcept = default;
    Triangle& operator=(Triangle&&) noexcept = default;
    ~Triangle() override = default;

    [[nodiscard]] std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Triangle>(*this);
    }

    [[nodiscard]] const char* shape_name() const { return "Triangle"; }

private:
    [[nodiscard]] bool is_equal(const Figure<T>& other) const override {
        return base_type::is_equal(other);
    }

    [[nodiscard]] static bool is_isosceles(
        const point_type& apex,
        const point_type& base_left,
        const point_type& base_right) {
        const auto dist_left = squared_distance(apex, base_left);
        const auto dist_right = squared_distance(apex, base_right);
        return almost_equal(dist_left, dist_right);
    }

    template <typename U>
    [[nodiscard]] static std::common_type_t<U, double> squared_distance(const Point<U>& lhs, const Point<U>& rhs) {
        using real = std::common_type_t<U, double>;
        const auto dx = static_cast<real>(lhs.x()) - static_cast<real>(rhs.x());
        const auto dy = static_cast<real>(lhs.y()) - static_cast<real>(rhs.y());
        return dx * dx + dy * dy;
    }
};

}  // namespace lab04
