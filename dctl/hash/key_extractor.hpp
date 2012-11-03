#pragma once
#include <cstddef>                      // size_t
#include <type_traits>                  // is_integral
#include <dctl/node/material.hpp>

namespace dctl {
namespace hash {

struct MaterialExtractor
{
        typedef Material key_type;

        template<typename U, typename Hash>
        key_type operator()(U const& u, Hash /* hash */) const
        {
                return (u.material());
        }
};

struct UpperHashBitsExtractor
{
        typedef std::size_t key_type;

        template<typename U, typename Hash>
        key_type operator()(U const& /* u */, Hash hash) const
        {
                static_assert(std::is_integral<Hash>::value,     "Bitwise shift only applicable to integral types.");
                static_assert(std::is_integral<key_type>::value, "Bitwise shift only applicable to integral types.");
                static_assert(sizeof(key_type) <= sizeof(Hash),  "Key cannot be of larger type than the hash.");

                return (static_cast<key_type>(hash >> (num_bits<Hash>::value - num_bits<key_type>::value)));
        }
};

}       // namespace hash
}       // namespace dctl
