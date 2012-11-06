#pragma once
#include <cstddef>                      // size_t
#include <type_traits>                  // is_integral
#include <dctl/node/material.hpp>

namespace dctl {
namespace hash {

struct MaterialExtractor
{
        typedef Material value_type;

        template<typename Key, typename Index>
        value_type operator()(Key const& key, Index /* index */) const
        {
                return (key.material());
        }
};

struct UpperHashBitsExtractor
{
        typedef std::size_t value_type;

        template<typename Key, typename Index>
        value_type operator()(Key const& /* key */, Index index) const
        {
                static_assert(std::is_integral<Index>::value,      "Bitwise shift only applicable to integral types.");
                static_assert(std::is_integral<value_type>::value, "Bitwise shift only applicable to integral types.");
                static_assert(sizeof(value_type) <= sizeof(Index), "Key cannot be of larger type than the hash.");

                return (static_cast<value_type>(index >> (num_bits<Index>::value - num_bits<value_type>::value)));
        }
};

}       // namespace hash
}       // namespace dctl
