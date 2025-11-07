#pragma once

#include <concepts>
#include <ostream>
#include <type_traits>

namespace lab04 {

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

template <Scalar T>
class Point {
public:
    using value_type = T;

    constexpr Point() = default;
    constexpr Point(T x, T y) noexcept : x_(x), y_(y) {}
    constexpr Point(const Point&) = default;
    constexpr Point(Point&&) noexcept = default;
    Point& operator=(const Point&) = default;
    Point& operator=(Point&&) noexcept = default;
    ~Point() = default;

    [[nodiscard]] constexpr T x() const noexcept { return x_; }
    [[nodiscard]] constexpr T y() const noexcept { return y_; }

    void set_x(T value) noexcept { x_ = value; }
    void set_y(T value) noexcept { y_ = value; }

    Point& operator+=(const Point& other) noexcept {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Point& operator-=(const Point& other) noexcept {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    Point& operator*=(T value) noexcept {
        x_ *= value;
        y_ *= value;
        return *this;
    }

    Point& operator/=(T value) {
        using common = std::common_type_t<T, double>;
        x_ = static_cast<T>(static_cast<common>(x_) / static_cast<common>(value));
        y_ = static_cast<T>(static_cast<common>(y_) / static_cast<common>(value));
        return *this;
    }

    [[nodiscard]] Point operator+(const Point& other) const noexcept {
        Point copy{*this};
        copy += other;
        return copy;
    }

    [[nodiscard]] Point operator-(const Point& other) const noexcept {
        Point copy{*this};
        copy -= other;
        return copy;
    }

    [[nodiscard]] Point operator*(T value) const noexcept {
        Point copy{*this};
        copy *= value;
        return copy;
    }

    [[nodiscard]] Point operator/(T value) const {
        Point copy{*this};
        copy /= value;
        return copy;
    }

private:
    T x_{};
    T y_{};

    friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << '(' << point.x_ << ", " << point.y_ << ')';
        return os;
    }
};

template <Scalar T>
[[nodiscard]] Point<T> operator*(T value, const Point<T>& point) noexcept {
    return point * value;
}

}  // namespace lab04
