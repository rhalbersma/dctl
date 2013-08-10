#pragma once
#include <cstddef>                      // size_t
#include <type_traits>                  // is_integral
#include <dctl/node/material.hpp>

namespace dctl {
namespace hash {

struct MaterialExtractor
{
        using result_type = Material;

        template<class Key, class Index>
        result_type const& operator()(Key const& key, Index /* index */) const
        {
                return key.material();
        }
};

struct UpperHashBitsExtractor
{
        using result_type = std::size_t;

        template<class Key, class Index>
        result_type operator()(Key const& /* key */, Index index) const
        {
                static_assert(std::is_integral<Index>::value,      "Bitwise shift only applicable to integral types.");
                static_assert(std::is_integral<result_type>::value, "Bitwise shift only applicable to integral types.");
                static_assert(sizeof(result_type) <= sizeof(Index), "Key cannot be of larger type than the hash.");

                return static_cast<result_type>(index >> (num_bits<Index>::value - num_bits<result_type>::value));
        }

        template<class Index>
        result_type operator()(Index index) const
        {
                static_assert(std::is_integral<Index>::value,      "Bitwise shift only applicable to integral types.");
                static_assert(std::is_integral<result_type>::value, "Bitwise shift only applicable to integral types.");
                static_assert(sizeof(result_type) <= sizeof(Index), "Key cannot be of larger type than the hash.");

                return static_cast<result_type>(index >> (num_bits<Index>::value - num_bits<result_type>::value));
        }
};

}       // namespace hash
}       // namespace dctl
