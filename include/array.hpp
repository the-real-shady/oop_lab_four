#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace lab04 {

template <typename T>
class Array {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    Array() = default;

    explicit Array(size_type initial_capacity) {
        reserve(initial_capacity);
    }

    Array(std::initializer_list<value_type> init) {
        reserve(init.size());
        for (const auto& value : init) {
            push_back(value);
        }
    }

    Array(const Array& other) {
        reserve(other.size_);
        const auto other_data = other.data();
        auto dest = data();
        for (size_type i = 0; i < other.size_; ++i) {
            dest[i] = other_data[i];
        }
        size_ = other.size_;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            Array copy{other};
            swap(copy);
        }
        return *this;
    }

    Array(Array&& other) noexcept
        : size_(std::exchange(other.size_, 0)),
          capacity_(std::exchange(other.capacity_, 0)),
          storage_(std::move(other.storage_)) {}

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            storage_ = std::move(other.storage_);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    ~Array() = default;

    [[nodiscard]] size_type size() const noexcept { return size_; }
    [[nodiscard]] size_type capacity() const noexcept { return capacity_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    reference operator[](size_type index) {
        if (index >= size_) {
            throw std::out_of_range("Array index out of range");
        }
        return data()[index];
    }

    const_reference operator[](size_type index) const {
        if (index >= size_) {
            throw std::out_of_range("Array index out of range");
        }
        return data()[index];
    }

    reference front() {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return (*this)[0];
    }

    const_reference front() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return (*this)[0];
    }

    reference back() {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return (*this)[size_ - 1];
    }

    const_reference back() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return (*this)[size_ - 1];
    }

    pointer data() noexcept { return storage_.get(); }
    const_pointer data() const noexcept { return storage_.get(); }

    iterator begin() noexcept { return data(); }
    const_iterator begin() const noexcept { return data(); }
    const_iterator cbegin() const noexcept { return data(); }

    iterator end() noexcept { return data() + size_; }
    const_iterator end() const noexcept { return data() + size_; }
    const_iterator cend() const noexcept { return data() + size_; }

    void clear() noexcept {
        pointer ptr = data();
        if (ptr != nullptr) {
            for (size_type i = 0; i < size_; ++i) {
                ptr[i] = value_type{};
            }
        }
        size_ = 0;
    }

    void reserve(size_type new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        auto new_storage = std::shared_ptr<value_type[]>(
            new value_type[new_capacity], std::default_delete<value_type[]>());

        const auto old_ptr = data();
        const auto new_ptr = new_storage.get();
        for (size_type i = 0; i < size_; ++i) {
            new_ptr[i] = std::move(old_ptr[i]);
        }

        storage_ = std::move(new_storage);
        capacity_ = new_capacity;
    }

    void push_back(const value_type& value) {
        ensure_capacity(size_ + 1);
        data()[size_++] = value;
    }

    void push_back(value_type&& value) {
        ensure_capacity(size_ + 1);
        data()[size_++] = std::move(value);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        ensure_capacity(size_ + 1);
        pointer ptr = data();
        ptr[size_] = value_type(std::forward<Args>(args)...);
        return ptr[size_++];
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        --size_;
        data()[size_] = value_type{};
    }

    void erase(size_type index) {
        if (index >= size_) {
            throw std::out_of_range("Array index out of range");
        }
        pointer ptr = data();
        for (size_type i = index; i + 1 < size_; ++i) {
            ptr[i] = std::move(ptr[i + 1]);
        }
        --size_;
        ptr[size_] = value_type{};
    }

    void swap(Array& other) noexcept {
        std::swap(storage_, other.storage_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    size_type size_{0};
    size_type capacity_{0};
    std::shared_ptr<value_type[]> storage_{};

    void ensure_capacity(size_type target) {
        if (target <= capacity_) {
            return;
        }
        const auto new_capacity = capacity_ == 0 ? target : std::max(target, capacity_ * 2);
        reserve(new_capacity);
    }
};

template <typename T>
void swap(Array<T>& lhs, Array<T>& rhs) noexcept {
    lhs.swap(rhs);
}

}  // namespace lab04
