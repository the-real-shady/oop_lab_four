#pragma once

#include <array>
#include <cmath>
#include <iomanip>
#include <limits>
#include <memory>
#include <type_traits>

#include "figure.hpp"

namespace lab04 {

template <Scalar T>
bool almost_equal(T lhs, T rhs) {
    if constexpr (std::is_floating_point_v<T>) {
        const auto diff = std::fabs(lhs - rhs);
        const auto scale = std::max({std::fabs(lhs), std::fabs(rhs), static_cast<T>(1)});
        return diff <= static_cast<T>(std::numeric_limits<T>::epsilon() * 16) * scale;
    }
    return lhs == rhs;
}

template <typename Derived, Scalar T, std::size_t VertexCount>
class PolygonFigure : public Figure<T> {
   public:
    using point_type = Point<T>;
    using vertices_array = std::array<std::unique_ptr<point_type>, VertexCount>;

    PolygonFigure() = default;

    explicit PolygonFigure(const std::array<point_type, VertexCount>& points) { assign(points); }

    PolygonFigure(const PolygonFigure& other) { copy_from(other); }
    PolygonFigure& operator=(const PolygonFigure& other) {
        if (this != &other) {
            copy_from(other);
        }
        return *this;
    }

    PolygonFigure(PolygonFigure&&) noexcept = default;
    PolygonFigure& operator=(PolygonFigure&&) noexcept = default;

    ~PolygonFigure() override = default;

    [[nodiscard]] point_type center() const override {
        using common = std::common_type_t<T, double>;
        common sum_x{};
        common sum_y{};
        for (const auto& vertex : vertices_) {
            sum_x += static_cast<common>(vertex->x());
            sum_y += static_cast<common>(vertex->y());
        }
        const auto count = static_cast<common>(VertexCount);
        return point_type{static_cast<T>(sum_x / count), static_cast<T>(sum_y / count)};
    }

    [[nodiscard]] double area() const override {
        long double result = 0.0L;
        for (std::size_t i = 0; i < VertexCount; ++i) {
            const auto& current = *vertices_[i];
            const auto& next = *vertices_[(i + 1) % VertexCount];
            result += static_cast<long double>(current.x()) * static_cast<long double>(next.y());
            result -= static_cast<long double>(current.y()) * static_cast<long double>(next.x());
        }
        return std::fabs(static_cast<double>(result) * 0.5);
    }

    void print(std::ostream& os) const override {
        const auto previous_flags = os.flags();
        const auto previous_precision = os.precision();

        os << static_cast<const Derived*>(this)->shape_name() << ": ";
        os << "vertices=[";
        for (std::size_t i = 0; i < VertexCount; ++i) {
            os << *vertices_[i];
            if (i + 1 < VertexCount) {
                os << ", ";
            }
        }
        os << "], center=" << center() << ", area=" << std::fixed << std::setprecision(3) << area();

        os.flags(previous_flags);
        os.precision(previous_precision);
    }

    [[nodiscard]] std::array<point_type, VertexCount> vertices() const {
        std::array<point_type, VertexCount> result{};
        for (std::size_t i = 0; i < VertexCount; ++i) {
            result[i] = *vertices_[i];
        }
        return result;
    }

   protected:
    vertices_array vertices_{};

    void assign(const std::array<point_type, VertexCount>& points) {
        for (std::size_t i = 0; i < VertexCount; ++i) {
            vertices_[i] = std::make_unique<point_type>(points[i]);
        }
    }

    void copy_from(const PolygonFigure& other) {
        for (std::size_t i = 0; i < VertexCount; ++i) {
            vertices_[i] = std::make_unique<point_type>(*other.vertices_[i]);
        }
    }

    [[nodiscard]] bool is_equal(const PolygonFigure& other) const {
        for (std::size_t i = 0; i < VertexCount; ++i) {
            if (!almost_equal(vertices_[i]->x(), other.vertices_[i]->x()) ||
                !almost_equal(vertices_[i]->y(), other.vertices_[i]->y())) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool is_equal(const Figure<T>& other) const override {
        auto other_ptr = dynamic_cast<const Derived*>(&other);
        if (!other_ptr) {
            return false;
        }
        return is_equal(*other_ptr);
    }
};

}  // namespace lab04
