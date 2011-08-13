#pragma once
#include <functional>   // std::unary_function

namespace dctl {
namespace hash {

template<typename Index, typename Key>
struct ShiftSign
: 
        public std::unary_function<Index, Key>
{
        Key operator()(Index) const;
};

template<typename Item, typename Key>
struct FindSign
: 
        public std::unary_function<Item, Key>
{
        const Key& operator()(const Item&) const;
};

}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "Sign.hpp"
