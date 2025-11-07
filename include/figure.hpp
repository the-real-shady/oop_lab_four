#pragma once

#include <memory>
#include <ostream>

#include "point.hpp"

namespace lab04 {

template <Scalar T>
class Figure {
public:
    using value_type = T;
    virtual ~Figure() = default;

    [[nodiscard]] virtual Point<T> center() const = 0;
    [[nodiscard]] virtual double area() const = 0;
    virtual void print(std::ostream& os) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Figure<T>> clone() const = 0;

    explicit operator double() const { return area(); }

    bool operator==(const Figure& other) const { return is_equal(other); }
    bool operator!=(const Figure& other) const { return !(*this == other); }

protected:
    [[nodiscard]] virtual bool is_equal(const Figure& other) const = 0;
};

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& figure) {
    figure.print(os);
    return os;
}

}  // namespace lab04

