#pragma once
#include <functional>

namespace Hash {

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

}       // namespace Hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Sign.hpp"