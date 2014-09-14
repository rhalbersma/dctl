#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/traits.hpp>                  // digits

namespace dctl {
namespace bit {
namespace detail {

template<>
class BaseSet<0>
{
private:
        using block_type = unsigned long long;
        static constexpr auto N = 0 * digits<block_type>;

public:
        // constructors

        constexpr BaseSet() = default;
        /* implicit */ constexpr BaseSet(unsigned long long) noexcept {}

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet& operator=(BaseSet const&) = default;

        // element access

        constexpr auto*       block_ptr(int)       { return this; }
        constexpr auto const* block_ptr(int) const { return this; }

        // data access

        constexpr auto*       do_data()       { return this; }
        constexpr auto const* do_data() const { return this; }

        // comparators

        constexpr auto do_equal                    (BaseSet const&) const noexcept { return true;  }
        constexpr auto do_colexicographical_compare(BaseSet const&) const noexcept { return false; }
        constexpr auto do_intersects               (BaseSet const&) const noexcept { return false; }
        constexpr auto do_is_subset_of             (BaseSet const&) const noexcept { return true;  }
        constexpr auto do_is_proper_subset_of      (BaseSet const&) const noexcept { return false; }

        // modifiers

        constexpr auto do_swap(BaseSet&) noexcept {}

        constexpr auto do_sanitize() noexcept {}

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
