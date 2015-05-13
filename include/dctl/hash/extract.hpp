#pragma once
#include <cstddef>                      // size_t
#include <limits>                       // digits
#include <type_traits>                  // is_integral

namespace dctl {
namespace extract {

struct Hash
{
        using result_type = std::size_t;

        template<class Key>
        auto operator()(Key const& key) const
        {
                return key.hash();
        }
};

struct ActiveColor
{
        template<class Key>
        auto operator()(Key const& key) const
        {
                return key.to_move();
        }
};

struct Material
{
        template<class Key, class Index>
        auto const& operator()(Key const& key, Index /* index */) const
        {
                return key.material();
        }
};

struct UpperBits
{
        using result_type = std::size_t;

        template<class Key, class Index>
        auto operator()(Key const& /* key */, Index index) const
        {
                static_assert(std::is_integral<Index>::value,       "Bitwise shift only applicable to integral types.");
                static_assert(std::is_integral<result_type>::value, "Bitwise shift only applicable to integral types.");
                static_assert(sizeof(result_type) <= sizeof(Index), "Key cannot be of larger type than the hash.");

                return static_cast<result_type>(index >> (std::numeric_limits<Index>::digits - std::numeric_limits<result_type>::digits));
        }
};

}       // namespace extract
}       // namespace dctl
