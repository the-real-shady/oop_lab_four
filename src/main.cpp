#include <iostream>
#include <limits>
#include <memory>
#include <string>

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

template <lab04::Scalar T>
T read_value(const std::string& prompt) {
    T value{};
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный ввод. Повторите попытку.\n";
    }
}

template <lab04::Scalar T>
Point<T> read_point(const std::string& prompt) {
    std::cout << prompt << '\n';
    const T x = read_value<T>("  x: ");
    const T y = read_value<T>("  y: ");
    return Point<T>(x, y);
}

template <lab04::Scalar T>
std::shared_ptr<Figure<T>> create_triangle() {
    const auto center = read_point<T>("Введите координаты центра треугольника");
    const T base_width = read_value<T>("Введите длину основания: ");
    const T height = read_value<T>("Введите высоту: ");
    return std::make_shared<Triangle<T>>(center, base_width, height);
}

template <lab04::Scalar T>
std::shared_ptr<Figure<T>> create_square() {
    const auto center = read_point<T>("Введите координаты центра квадрата");
    const T side = read_value<T>("Введите длину стороны: ");
    return std::make_shared<Square<T>>(center, side);
}

template <lab04::Scalar T>
std::shared_ptr<Figure<T>> create_rectangle() {
    const auto center = read_point<T>("Введите координаты центра прямоугольника");
    const T width = read_value<T>("Введите ширину: ");
    const T height = read_value<T>("Введите высоту: ");
    return std::make_shared<Rectangle<T>>(center, width, height);
}

void print_menu() {
    std::cout << "\nМеню:\n"
              << "1. Добавить треугольник\n"
              << "2. Добавить квадрат\n"
              << "3. Добавить прямоугольник\n"
              << "4. Вывести все фигуры\n"
              << "5. Вычислить суммарную площадь\n"
              << "6. Вывести центры фигур\n"
              << "7. Удалить фигуру по индексу\n"
              << "8. Показать емкость и размер массива\n"
              << "9. Демонстрация шаблона массива\n"
              << "0. Выход\n";
}

template <lab04::Scalar T>
void print_figures(const Array<std::shared_ptr<Figure<T>>>& figures) {
    if (figures.empty()) {
        std::cout << "Массив фигур пуст.\n";
        return;
    }
    for (std::size_t i = 0; i < figures.size(); ++i) {
        const auto& figure = figures[i];
        std::cout << i << ": ";
        if (figure) {
            std::cout << *figure << '\n';
        } else {
            std::cout << "<пусто>\n";
        }
    }
}

template <lab04::Scalar T>
void print_centers(const Array<std::shared_ptr<Figure<T>>>& figures) {
    if (figures.empty()) {
        std::cout << "Массив фигур пуст.\n";
        return;
    }
    for (std::size_t i = 0; i < figures.size(); ++i) {
        const auto& figure = figures[i];
        std::cout << i << ": ";
        if (figure) {
            std::cout << "центр = " << figure->center()
                      << ", площадь = " << figure->area() << '\n';
        } else {
            std::cout << "<пусто>\n";
        }
    }
}

template <lab04::Scalar T>
double total_area(const Array<std::shared_ptr<Figure<T>>>& figures) {
    double total = 0.0;
    for (const auto& figure : figures) {
        if (figure) {
            total += figure->area();
        }
    }
    return total;
}

void demonstrate_array_templates() {
    static auto triangle_holder =
        std::make_unique<Triangle<int>>(Point<int>(0, 0), 4, 6);

    Array<Figure<int>*> figure_ptr_array;
    figure_ptr_array.push_back(triangle_holder.get());

    Array<Square<int>> squares;
    squares.emplace_back(Point<int>(0, 0), 4);

    std::cout << "Демонстрация Array<Figure<int>*>: размер = "
              << figure_ptr_array.size() << '\n';
    std::cout << "Демонстрация Array<Square<int>>: площадь первой фигуры = "
              << squares[0].area() << '\n';
}

}  // namespace

int main() {
    using value_type = double;
    Array<std::shared_ptr<Figure<value_type>>> figures;

    bool running = true;
    while (running) {
        print_menu();
        const int choice = read_value<int>("Выберите пункт меню: ");

        try {
            switch (choice) {
                case 1: {
                    auto figure = create_triangle<value_type>();
                    figures.push_back(std::move(figure));
                    std::cout << "Треугольник добавлен.\n";
                    break;
                }
                case 2: {
                    auto figure = create_square<value_type>();
                    figures.push_back(std::move(figure));
                    std::cout << "Квадрат добавлен.\n";
                    break;
                }
                case 3: {
                    auto figure = create_rectangle<value_type>();
                    figures.push_back(std::move(figure));
                    std::cout << "Прямоугольник добавлен.\n";
                    break;
                }
                case 4:
                    print_figures(figures);
                    break;
                case 5:
                    std::cout << "Суммарная площадь = " << total_area(figures) << '\n';
                    break;
                case 6:
                    print_centers(figures);
                    break;
                case 7: {
                    if (figures.empty()) {
                        std::cout << "Массив пуст.\n";
                        break;
                    }
                    const auto index = read_value<std::size_t>("Введите индекс для удаления: ");
                    figures.erase(index);
                    std::cout << "Фигура удалена.\n";
                    break;
                }
                case 8:
                    std::cout << "Размер = " << figures.size()
                              << ", емкость = " << figures.capacity() << '\n';
                    break;
                case 9:
                    demonstrate_array_templates();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Неизвестный пункт меню.\n";
                    break;
            }
        } catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << '\n';
        }
    }

    std::cout << "Программа завершена.\n";
    return 0;
}

