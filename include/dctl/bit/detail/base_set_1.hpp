#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/intrinsic.hpp>               // popcount
#include <dctl/bit/limits.hpp>                  // digits, is_unsigned_integer
#include <dctl/bit/masks.hpp>                   // none, one, all
#include <cassert>                              // assert
#include <memory>                               // addressof
#include <utility>                              // swap

namespace dctl {
namespace bit {
namespace detail {

template<class Block>
class BaseSet<Block, 1>
{
        static_assert(is_unsigned_integer<Block>, "");
        static constexpr auto N = 1 * digits<Block>;
public:
        // constructors

        /* implicit */ constexpr BaseSet(Block value) noexcept
        :
                elems{value}
        {}

        constexpr BaseSet() = default;

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet(BaseSet&&) = default;
        BaseSet& operator=(BaseSet const&) = default;
        BaseSet& operator=(BaseSet&&) = default;

        // data access

        constexpr auto* block_begin() noexcept
        {
                return std::addressof(elems);
        }

        constexpr auto const* block_begin() const noexcept
        {
                return std::addressof(elems);
        }

        constexpr auto* block_end() noexcept
        {
                return std::addressof(elems);
        }

        constexpr auto const* block_end() const noexcept
        {
                return std::addressof(elems);
        }

        constexpr auto& block_back() noexcept
        {
                return elems;
        }

        constexpr auto const& block_back() const noexcept
        {
                return elems;
        }

        constexpr auto& block_ref(int /* n */)
        {
                //assert(0 <= n && n < N);
                return elems;
        }

        constexpr auto const& block_ref(int /* n */) const
        {
                //assert(0 <= n && n < N);
                return elems;
        }

        // comparators

        constexpr auto do_equal(BaseSet const& other) const noexcept
        {
                return elems == other.elems;
        }

        constexpr auto do_colexicographical_compare(BaseSet const& other) const noexcept
        {
                return elems < other.elems;
        }

        constexpr auto do_intersects(BaseSet const& other) const noexcept
        {
                return (elems & other.elems) != masks::none<Block>;
        }

        constexpr auto do_is_subset_of(BaseSet const& other) const noexcept
        {
                return (elems & ~other.elems) == masks::none<Block>;
        }

        constexpr auto do_is_proper_subset_of(BaseSet const& other) const noexcept
        {
                if (elems & ~other.elems)
                        return false;
                return (~elems & other.elems) != masks::none<Block>;
        }

        // modifiers

        /* constexpr */ auto do_swap(BaseSet& other) noexcept
        {
                using std::swap;
                swap(elems, other.elems);
        }

        constexpr auto do_set() noexcept
        {
                elems = masks::all<Block>;
        }

        constexpr auto do_reset() noexcept
        {
                elems = masks::none<Block>;
        }

        constexpr auto do_flip() noexcept
        {
                elems = ~elems;
        }

        constexpr auto do_and(BaseSet const& other) noexcept
        {
                elems &= other.elems;
        }

        constexpr auto do_or(BaseSet const& other) noexcept
        {
                elems |= other.elems;
        }

        constexpr auto do_xor(BaseSet const& other) noexcept
        {
                elems ^= other.elems;
        }

        constexpr auto do_minus(BaseSet const& other) noexcept
        {
                elems &= ~other.elems;
        }

        constexpr auto do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                elems <<= n;
        }

        constexpr auto do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                elems >>= n;
        }

        // observers

        constexpr auto do_all() const noexcept
        {
                return elems == masks::all<Block>;
        }

        constexpr auto do_any() const noexcept
        {
                return elems != masks::none<Block>;
        }

        constexpr auto do_none() const noexcept
        {
                return elems == masks::none<Block>;
        }

        constexpr auto do_count() const noexcept
        {
                return bit::intrinsic::popcount(elems);
        }

private:
        // representation

        Block elems{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
