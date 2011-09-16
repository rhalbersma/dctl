#pragma once
#include <functional>                   // std::unary_function
#include <type_traits>                  // std::is_integral

namespace dctl {
namespace hash {

template<typename Index, typename Key>
struct ShiftSign
: 
        public std::unary_function<Index, Key>
{
        Key operator()(Index index) const
        {
                static_assert(std::is_integral<Index>::value, "Bitwise shift only applicable to integer types.");
                static_assert(std::is_integral<Key>::value, "Bitwise shift only applicable to integer types.");
                static_assert(sizeof(Index) >= sizeof(Key), "Hash key cannot be of larger type than the hash index.");

                return static_cast<Key>(index >> 8 * (sizeof(Index) - sizeof(Key)));
        }
};

template<typename Item, typename Key>
struct FindSign
: 
        public std::unary_function<Item, Key>
{
        const Key& operator()(const Item& item) const
        {
                static_assert(!std::is_integral<Item>::value, "Only bitwise shift applicable to integer types.");

                return item.key();
        }
};

}       // namespace hash
}       // namespace dctl
