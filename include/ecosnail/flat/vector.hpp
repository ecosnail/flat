#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ecosnail::flat {

template <class T>
struct Vector {
    // construction

    Vector()
        : x{}, y{}
    { }

    Vector(T x, T y)
        : x(std::move(x)), y(std::move(y))
    { }

    // implicit conversions

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector(const Vector<U>& rhs)
        : x(rhs.x), y(rhs.y)
    { }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector(Vector<U>&& rhs)
        : x(std::move(rhs.x)), y(std::move(rhs.y))
    { }

    // assignment

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator=(const Vector<U>& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator=(const Vector<U>&& rhs)
    {
        x = std::move(rhs.x);
        y = std::move(rhs.y);
        return *this;
    }

    // array subscript operator

    T& operator[](std::size_t idx)
    {
        assert(idx < 2);
        static const T Vector::* ptrs[] {&Vector::x, &Vector::y};
        return this->*ptrs[idx];
    }

    const T& operator[](std::size_t idx) const
    {
        assert(idx < 2);
        static const T Vector::* ptrs[] {&Vector::x, &Vector::y};
        return this->*ptrs[idx];
    }

    // arithmetic operators

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator+=(const Vector<U>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator-=(const Vector<U>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator*=(const U& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Vector& operator/=(const U& scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    T x;
    T y;
};

// arithmetic operators

template <class L, class R>
auto operator+(const Vector<L>& lhs, const Vector<R>& rhs)
{
    return Vector<std::common_type_t<L, R>>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <class L, class R>
auto operator-(const Vector<L>& lhs, const Vector<R>& rhs)
{
    return Vector<std::common_type_t<L, R>>{lhs.x - rhs.x, lhs.y - rhs.y};
};

template <class T, class U>
auto operator*(const Vector<T>& vector, const U& scalar)
{
    return Vector<std::common_type_t<T, U>>{
        vector.x * scalar, vector.y * scalar};
}

template <class T, class U>
Vector<std::common_type_t<T, U>> operator*(
    const U& scalar, const Vector<T>& vector)
{
    return vector * scalar;
}

template <class T, class U>
auto operator/(const Vector<T>& vector, const U& scalar)
{
    return Vector<std::common_type_t<T, U>>{
        vector.x / scalar, vector.y / scalar};
}

// relational operators

template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return lhs.x <= rhs.x && lhs.y <= rhs.y;
}

template <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return rhs <= lhs;
}

template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return lhs <= rhs && lhs != rhs;
}

template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return rhs < lhs;
}

// geometry functions

template <class T>
T length(const Vector<T>& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

template <class T>
Vector<T> normalized(const Vector<T>& v)
{
    auto l = length(v);
    if (l == 0) {
        return {};
    } else {
        return v / l;
    }
}

// stream output

template <class T>
std::ostream& operator<<(std::ostream& output, const Vector<T>& vector)
{
    return output << vector.x << ", " << vector.y;
}

} // namespace ecosnail::flat

namespace std {

template <class T>
struct less<ecosnail::flat::Vector<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Vector<T>& lhs,
        const ecosnail::flat::Vector<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }
};

template <class T>
struct greater<ecosnail::flat::Vector<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Vector<T>& lhs,
        const ecosnail::flat::Vector<T>& rhs)
    {
        return less<ecosnail::flat::Vector<T>>(rhs, lhs)();
    }
};

template <class T>
struct less_equal<ecosnail::flat::Vector<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Vector<T>& lhs,
        const ecosnail::flat::Vector<T>& rhs)
    {
        return !greater<ecosnail::flat::Vector<T>>(lhs, rhs);
    }
};

template <class T>
struct greater_equal<ecosnail::flat::Vector<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Vector<T>& lhs,
        const ecosnail::flat::Vector<T>& rhs)
    {
        return !less<ecosnail::flat::Vector<T>>(lhs, rhs);
    }
};

} // namespace std
