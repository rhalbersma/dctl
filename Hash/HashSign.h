#pragma once
#include <functional>

template<typename Index, typename Key>
struct ShiftSign: public std::unary_function<Index, Key>
{
        Key operator()(Index) const;
};

template<typename Item, typename Key>
struct FindSign: public std::unary_function<Item, Key>
{
        const Key& operator()(const Item&) const;
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "HashSign.hpp"
