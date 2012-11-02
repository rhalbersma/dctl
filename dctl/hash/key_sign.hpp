#pragma once
#include <functional>                   // function
#include <type_traits>                  // is_integral
#include <dctl/utility/int.hpp>

namespace dctl {
namespace hash {

template<typename Key, typename Item>
struct FindKey
:
        std::function<Key(Item)>
{
        Key const& operator()(Item const& item) const
        {
                static_assert(!std::is_integral<Item>::value, "Only bitwise shift applicable to integer types.");

                return (item.key());
        }
};

template<typename Key, typename Index>
struct ShiftKey
:
        std::function<Key(Index)>
{
        Key operator()(Index index) const
        {
                static_assert(std::is_integral<Index>::value, "Bitwise shift only applicable to integer types.");
                static_assert(std::is_integral<Key>::value, "Bitwise shift only applicable to integer types.");
                static_assert(sizeof(Key) <= sizeof(Index), "Key cannot be of larger type than the index.");

                return (static_cast<Key>(index >> (num_bits<Index>::value - num_bits<Key>::value)));
        }
};

}       // namespace hash
}       // namespace dctl
