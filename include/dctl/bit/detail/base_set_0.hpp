#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/traits.hpp>                  // is_unsigned_integer

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
class BaseSet<Block, 0>
{
        static_assert(is_unsigned_integer<Block>, "");
public:
        // constructors

        constexpr BaseSet() = default;
        /* implicit */ constexpr BaseSet(Block) noexcept {}

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet& operator=(BaseSet const&) = default;

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

        constexpr auto do_equal                    (BaseSet const&) const noexcept { return true;  }
        constexpr auto do_colexicographical_compare(BaseSet const&) const noexcept { return false; }
        constexpr auto do_intersects               (BaseSet const&) const noexcept { return false; }
        constexpr auto do_is_subset_of             (BaseSet const&) const noexcept { return true;  }
        constexpr auto do_is_proper_subset_of      (BaseSet const&) const noexcept { return false; }

        // modifiers

        constexpr auto do_swap(BaseSet&) noexcept {}

        constexpr auto do_set  () noexcept {}
        constexpr auto do_reset() noexcept {}
        constexpr auto do_flip () noexcept {}

        constexpr auto do_and  (BaseSet const&) noexcept {}
        constexpr auto do_or   (BaseSet const&) noexcept {}
        constexpr auto do_xor  (BaseSet const&) noexcept {}
        constexpr auto do_minus(BaseSet const&) noexcept {}

        constexpr auto do_left_shift (int) {}
        constexpr auto do_right_shift(int) {}

        // observers

        constexpr auto do_all  () const noexcept { return false; }
        constexpr auto do_any  () const noexcept { return false; }
        constexpr auto do_none () const noexcept { return true;  }
        constexpr auto do_count() const noexcept { return 0;     }
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
