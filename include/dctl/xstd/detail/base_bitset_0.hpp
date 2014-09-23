#pragma once
#include <dctl/xstd/detail/base_bitset_fwd.hpp> // base_bitset
#include <dctl/xstd/limits.hpp>                 // is_unsigned_integer

namespace xstd {
namespace detail {

template<class Block>
class base_bitset<Block, 0>
{
        static_assert(is_unsigned_integer<Block>, "");
public:
        // constructors

        constexpr base_bitset() = default;
        /* implicit */ constexpr base_bitset(Block) noexcept {}

protected:
        // destructor

        ~base_bitset() = default;

public:
        // copying, moving and assignment

        base_bitset(base_bitset const&) = default;
        base_bitset(base_bitset&&) = default;
        base_bitset& operator=(base_bitset const&) = default;
        base_bitset& operator=(base_bitset&&) = default;

        // data access

        constexpr auto*       block_begin()       noexcept { return  this; }
        constexpr auto const* block_begin() const noexcept { return  this; }
        constexpr auto*       block_end  ()       noexcept { return  this; }
        constexpr auto const* block_end  () const noexcept { return  this; }

        constexpr auto&       block_back()          noexcept { return *this; }
        constexpr auto const& block_back()    const noexcept { return *this; }
        constexpr auto&       block_ref (int)       noexcept { return *this; }
        constexpr auto const& block_ref (int) const noexcept { return *this; }

        // comparators

        constexpr auto do_equal                    (base_bitset const&) const noexcept { return true;  }
        constexpr auto do_colexicographical_compare(base_bitset const&) const noexcept { return false; }
        constexpr auto do_intersects               (base_bitset const&) const noexcept { return false; }
        constexpr auto do_is_subset_of             (base_bitset const&) const noexcept { return true;  }
        constexpr auto do_is_proper_subset_of      (base_bitset const&) const noexcept { return false; }

        // modifiers

        constexpr auto do_swap(base_bitset&) noexcept {}

        constexpr auto do_set  () noexcept {}
        constexpr auto do_reset() noexcept {}
        constexpr auto do_flip () noexcept {}

        constexpr auto do_and  (base_bitset const&) noexcept {}
        constexpr auto do_or   (base_bitset const&) noexcept {}
        constexpr auto do_xor  (base_bitset const&) noexcept {}
        constexpr auto do_minus(base_bitset const&) noexcept {}

        constexpr auto do_left_shift (int) {}
        constexpr auto do_right_shift(int) {}

        // observers

        constexpr auto do_all  () const noexcept { return false; }
        constexpr auto do_any  () const noexcept { return false; }
        constexpr auto do_none () const noexcept { return true;  }
        constexpr auto do_count() const noexcept { return 0;     }
};

}       // namespace detail
}       // namespace xstd
