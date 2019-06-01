#pragma once

#include <ecosnail/flat/vector.hpp>

#include <cassert>
#include <cstddef>
#include <ostream>
#include <type_traits>
#include <utility>

namespace ecosnail::flat {

template <class T>
struct Point {
    // construction

    Point()
        : x{}, y{}
    { }

    Point(T x, T y)
        : x(std::move(x)), y(std::move(y))
    { }

    // implicit conversions

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point(const Point<U>& rhs)
        : x(rhs.x), y(rhs.y)
    { }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point(Point<U>&& rhs)
        : x(std::move(rhs.x)), y(std::move(rhs.y))
    { }

    // assignment

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point& operator=(const Point<U>& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point& operator=(const Point<U>&& rhs)
    {
        x = std::move(rhs.x);
        y = std::move(rhs.y);
        return *this;
    }

    // array subscript operator

    T& operator[](std::size_t idx)
    {
        assert(idx < 2);
        static const T Point::* ptrs[] {&Point::x, &Point::y};
        return this->*ptrs[idx];
    }

    const T& operator[](std::size_t idx) const
    {
        assert(idx < 2);
        static const T Point::* ptrs[] {&Point::x, &Point::y};
        return this->*ptrs[idx];
    }

    // arithmetic operators

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point& operator+=(const Vector<U>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template <class U, class = std::enable_if_t<std::is_convertible_v<U, T>>>
    Point& operator-=(const Vector<U>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    T x;
    T y;
};

// arithmetic operators

template <class L, class R>
auto operator+(const Point<L>& lhs, const Vector<R>& rhs)
{
    return Point<std::common_type_t<L, R>>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <class L, class R>
auto operator-(const Point<L>& lhs, const Vector<R>& rhs)
{
    return Point<std::common_type_t<L, R>>{lhs.x - rhs.x, lhs.y - rhs.y};
};

template <class L, class R>
auto operator-(const Point<L>& lhs, const Point<R>& rhs)
{
    return Vector<std::common_type_t<L, R>>{lhs.x - rhs.x, lhs.y - rhs.y};
}

// relational operators

template <class T>
bool operator==(const Point<T>& lhs, const Point<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <class T>
bool operator!=(const Point<T>& lhs, const Point<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator<=(const Point<T>& lhs, const Point<T>& rhs)
{
    return lhs.x <= rhs.x && lhs.y <= rhs.y;
}

template <class T>
bool operator>=(const Point<T>& lhs, const Point<T>& rhs)
{
    return rhs <= lhs;
}

template <class T>
bool operator<(const Point<T>& lhs, const Point<T>& rhs)
{
    return lhs <= rhs && lhs != rhs;
}

template <class T>
bool operator>(const Point<T>& lhs, const Point<T>& rhs)
{
    return rhs < lhs;
}

// stream output

template <class T>
std::ostream& operator<<(std::ostream& output, const Point<T>& point)
{
    return output << point.x << ", " << point.y;
}

} // namespace ecosnail::flat

namespace std {

template <class T>
struct less<ecosnail::flat::Point<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Point<T>& lhs,
        const ecosnail::flat::Point<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }
};

template <class T>
struct greater<ecosnail::flat::Point<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Point<T>& lhs,
        const ecosnail::flat::Point<T>& rhs)
    {
        return less<ecosnail::flat::Point<T>>(rhs, lhs)();
    }
};

template <class T>
struct less_equal<ecosnail::flat::Point<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Point<T>& lhs,
        const ecosnail::flat::Point<T>& rhs)
    {
        return !greater<ecosnail::flat::Point<T>>(lhs, rhs);
    }
};

template <class T>
struct greater_equal<ecosnail::flat::Point<T>> {
    constexpr bool operator()(
        const ecosnail::flat::Point<T>& lhs,
        const ecosnail::flat::Point<T>& rhs)
    {
        return !less<ecosnail::flat::Point<T>>(lhs, rhs);
    }
};

} // namespace ecosnail::flat
