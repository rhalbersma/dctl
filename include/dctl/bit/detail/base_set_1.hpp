#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/intrinsic.hpp>        // popcount
#include <dctl/bit/traits.hpp>                  // all, any, none
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <limits>                               // digits
#include <utility>                              // swap

namespace dctl {
namespace bit {
namespace detail {

template<class UnsignedInteger>
class BaseSet<UnsignedInteger, 1>
{
private:
        static_assert(
                !std::numeric_limits<UnsignedInteger>::is_signed &&
                 std::numeric_limits<UnsignedInteger>::is_integer,
                "Template parameter 'T' in 'BaseSet<T, 1>' shall be of unsigned integer type."
        );

        static constexpr auto digits = std::numeric_limits<UnsignedInteger>::digits;
        static constexpr auto N = 1 * digits;

public:
        // constructors

        constexpr BaseSet() = default;

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet& operator=(BaseSet const&) = default;

        // element access

        constexpr auto* block_ptr(std::size_t /* n */)
        {
                return &elems;
        }

        constexpr auto const* block_ptr(std::size_t /* n */) const
        {
                return &elems;
        }

        // data access

        constexpr auto* do_data()
        {
                return &elems;
        }

        constexpr auto const* do_data() const
        {
                return &elems;
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
                return (elems & other.elems) != none<UnsignedInteger>;
        }

        constexpr auto do_is_subset_of(BaseSet const& other) const noexcept
        {
                return (elems & ~other.elems) == none<UnsignedInteger>;
        }

        constexpr auto do_is_proper_subset_of(BaseSet const& other) const noexcept
        {
                if (elems & ~other.elems)
                        return false;
                return (~elems & other.elems) != none<UnsignedInteger>;
        }

        // modifiers

        /* constexpr */ auto do_swap(BaseSet& other) noexcept
        {
                using std::swap;
                swap(elems, other.elems);
        }

        constexpr auto do_set() noexcept
        {
                elems = all<UnsignedInteger>;
        }

        constexpr auto do_reset() noexcept
        {
                elems = none<UnsignedInteger>;
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

        constexpr auto do_left_shift(std::size_t n)
        {
                assert(n < N);
                elems <<= n;
        }

        constexpr auto do_right_shift(std::size_t n)
        {
                assert(n < N);
                elems >>= n;
        }

        // observers

        constexpr auto do_all() const noexcept
        {
                return elems == all<UnsignedInteger>;
        }

        constexpr auto do_any() const noexcept
        {
                return elems != none<UnsignedInteger>;
        }

        constexpr auto do_none() const noexcept
        {
                return elems == none<UnsignedInteger>;
        }

        constexpr auto do_count() const noexcept
        {
                return bit::intrinsic::popcount(elems);
        }

private:
        // representation

        UnsignedInteger elems{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
