#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <stdexcept>

#include "../include/array.hpp"
#include "../include/rectangle.hpp"
#include "../include/square.hpp"
#include "../include/triangle.hpp"

namespace {

using lab04::Array;
using lab04::Figure;
using lab04::Point;
using lab04::Rectangle;
using lab04::Square;
using lab04::Triangle;

constexpr double kTolerance = 1e-6;

struct EmplaceableValue {
    int payload{0};
    EmplaceableValue() = default;
    explicit EmplaceableValue(int value) : payload(value) {}
};

TEST(PointOperationsTest, AdditionCombinesCoordinatesComponentWise) {
    const Point<int> lhs{1, 2};
    const Point<int> rhs{3, 4};
    const auto result = lhs + rhs;

    EXPECT_EQ(result.x(), 4);
    EXPECT_EQ(result.y(), 6);
}

TEST(PointOperationsTest, ScalarMultiplicationAndDivisionPreserveRatios) {
    Point<double> point{6.0, -3.0};
    const auto scaled = point * 0.5;
    const auto divided = point / 3.0;

    EXPECT_NEAR(scaled.x(), 3.0, kTolerance);
    EXPECT_NEAR(scaled.y(), -1.5, kTolerance);
    EXPECT_NEAR(divided.x(), 2.0, kTolerance);
    EXPECT_NEAR(divided.y(), -1.0, kTolerance);
}

TEST(TriangleGeometricPropertiesTest, CalculatesAreaAndCenterForIsoscelesTriangle) {
    Triangle<double> triangle(Point<double>(0.0, 0.0), 6.0, 4.0);

    EXPECT_NEAR(triangle.area(), 12.0, kTolerance);
    const auto center = triangle.center();
    EXPECT_NEAR(center.x(), 0.0, kTolerance);
    EXPECT_NEAR(center.y(), 0.0, kTolerance);
}

TEST(TriangleGeometricPropertiesTest, RejectsNonIsoscelesTriangles) {
    const Point<double> apex{0.0, 3.0};
    const Point<double> base_left{-2.0, 0.0};
    const Point<double> base_right{5.0, 0.0};
    EXPECT_THROW((Triangle<double>(apex, base_left, base_right)), std::invalid_argument);
}

TEST(TriangleCopySemanticsTest, CloneProducesIndependentInstance) {
    Triangle<double> triangle(Point<double>(1.0, 1.0), 8.0, 6.0);
    const auto clone = triangle.clone();

    ASSERT_NE(clone.get(), nullptr);
    EXPECT_NE(clone.get(), &triangle);
    EXPECT_TRUE(triangle == *clone);
}

TEST(SquareGeometricPropertiesTest, ComputesAreaAndCenterPrecisely) {
    Square<double> square(Point<double>(1.0, 1.0), 4.0);

    EXPECT_NEAR(square.area(), 16.0, kTolerance);
    EXPECT_NEAR(square.center().x(), 1.0, kTolerance);
    EXPECT_NEAR(square.center().y(), 1.0, kTolerance);
}

TEST(SquareGeometricPropertiesTest, InscribedCircleRadiusMatchesSideLength) {
    Square<double> square(Point<double>(0.0, 0.0), 2.0);
    EXPECT_NEAR(square.inscribed_circle_radius(), std::sqrt(2.0), kTolerance);
}

TEST(SquareComparisonTest, EqualityDependsOnVertexCoordinates) {
    Square<double> reference(Point<double>(0.0, 0.0), 4.0);
    Square<double> translated(Point<double>(1.0, 1.0), 4.0);
    Square<double> identical(Point<double>(0.0, 0.0), 4.0);

    EXPECT_TRUE(reference == identical);
    EXPECT_FALSE(reference == translated);
}

TEST(RectangleGeometricPropertiesTest, ComputesAreaAndCenter) {
    Rectangle<double> rectangle(Point<double>(2.0, -2.0), 6.0, 4.0);

    EXPECT_NEAR(rectangle.area(), 24.0, kTolerance);
    EXPECT_NEAR(rectangle.center().x(), 2.0, kTolerance);
    EXPECT_NEAR(rectangle.center().y(), -2.0, kTolerance);
}

TEST(RectangleGeometricPropertiesTest, CircumscribedCircleUsesDiagonalLength) {
    Rectangle<double> rectangle(Point<double>(0.0, 0.0), 6.0, 4.0);
    EXPECT_NEAR(rectangle.circumscribed_circle_radius(), std::sqrt(52.0) / 2.0, kTolerance);
}

TEST(FigureConversionTest, StaticCastToDoubleReturnsArea) {
    Square<double> square(Point<double>(0.0, 0.0), 3.0);
    const Figure<double>& figure = square;
    EXPECT_NEAR(static_cast<double>(figure), 9.0, kTolerance);
}

TEST(ArrayStorageTest, PushBackExpandsCapacityAndPreservesOrder) {
    Array<int> values;
    for (int i = 0; i < 10; ++i) {
        values.push_back(i);
    }

    ASSERT_EQ(values.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(values[i], i) << "Element at index " << i << " should remain in order.";
    }
    EXPECT_GE(values.capacity(), values.size());
}

TEST(ArrayStorageTest, EraseRemovesElementAndCompactsTail) {
    Array<int> values;
    values.push_back(100);
    values.push_back(200);
    values.push_back(300);
    values.erase(1);

    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 100);
    EXPECT_EQ(values[1], 300);
}

TEST(ArrayStorageTest, EraseThrowsOnOutOfRangeIndex) {
    Array<int> values;
    values.push_back(1);
    values.push_back(2);
    EXPECT_THROW(values.erase(5), std::out_of_range);
}

TEST(ArrayStorageTest, EmplaceBackConstructsValueInPlace) {
    Array<EmplaceableValue> values;
    values.emplace_back(42);
    values.emplace_back(-7);

    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0].payload, 42);
    EXPECT_EQ(values[1].payload, -7);
}

TEST(ArrayStorageTest, StoresSharedPointersToAbstractFigures) {
    Array<std::shared_ptr<Figure<double>>> figures;
    figures.push_back(std::make_shared<Square<double>>(Point<double>(0.0, 0.0), 2.0));
    figures.push_back(std::make_shared<Rectangle<double>>(Point<double>(0.0, 0.0), 2.0, 4.0));

    ASSERT_EQ(figures.size(), 2);
    const double combined_area =
        static_cast<double>(*figures[0]) + static_cast<double>(*figures[1]);
    EXPECT_NEAR(combined_area, 12.0, kTolerance);
}

}  // namespace
