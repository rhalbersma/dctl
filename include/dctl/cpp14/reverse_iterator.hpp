#pragma once
#include <iterator>

namespace cpp14 {

template<class T>
auto rbegin(T& t)
{
        return t.rbegin();
}

template<class T>
auto rend(T& t)
{
        return t.rend();
}

template<class T, size_t N>
auto rbegin(T (&array)[N])
{
        return std::reverse_iterator<T*>(std::end(array));
}

template<class T, size_t N>
auto rend(T (&array)[N])
{
        return std::reverse_iterator<T*>(std::begin(array));
}

}       // namespace cpp14
