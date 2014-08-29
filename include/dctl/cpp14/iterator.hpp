#pragma once
#include <cstddef>                      // size_t
#include <iterator>                     // begin, end, reverse_iterator

namespace cpp14 {

template<class C>
constexpr auto cbegin(C const& c)
{
        return std::begin(c);
}

template<class C>
constexpr auto cend(C const& c)
{
        return std::end(c);
}

template<class C>
auto rbegin(C& c)
{
        return c.rbegin();
}

template<class T, std::size_t N>
constexpr auto rbegin(T (&array)[N]) noexcept
{
        return std::reverse_iterator<T*>(std::end(array));
}

template<class C>
constexpr auto crbegin(C const& c)
{
        return cpp14::rbegin(c);
}

template<class C>
auto rend(C& c)
{
        return c.rend();
}

template<class T, std::size_t N>
constexpr auto rend(T (&array)[N]) noexcept
{
        return std::reverse_iterator<T*>(std::begin(array));
}

template<class C>
constexpr auto crend(C const& c)
{
        return cpp14::rend(c);
}

}       // namespace cpp14

namespace cpp17 {

// capacity:

template <class C>
constexpr auto size(C const& c) noexcept
{
        return c.size();
}

template <class T, std::size_t N>
constexpr std::size_t size(T const (&)[N]) noexcept
{
        return N;
}

template <class C>
constexpr bool empty(C const& c) noexcept
{
        return c.empty();
}

template <class T, std::size_t N>
constexpr bool empty(T const (&)[N]) noexcept
{
        return false;
}

// element access:

template <class C>
constexpr decltype(auto) front(C& c)
{
        return c.front();
}

template <class C>
constexpr decltype(auto) front(C const& c)
{
        return c.front();
}

template <class C>
constexpr decltype(auto) back(C& c)
{
        return c.back();
}

template <class C>
constexpr decltype(auto) back(C const& c)
{
        return c.back();
}

template <class T, std::size_t N>
constexpr T& front(T (&array)[N]) noexcept
{
        return array[0];
}

template <class T, std::size_t N>
constexpr T& back(T (&array)[N]) noexcept
{
        return array[N];
}

// data access:

template <class C>
constexpr auto data(C& c) noexcept
{
        return c.data();
}

template <class C>
constexpr auto data(C const& c) noexcept
{
        return c.data();
}

template <class T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept
{
        return array;
}

}       // namespace cpp17
