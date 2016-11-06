#pragma once
#include <type_traits>  // conditional, is_empty, is_pod

namespace dctl {
namespace util {
namespace block_adl {

template<class Tag>
struct tagged_empty
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = tagged_empty<Tag>;
                static_assert(std::is_empty<T>{});
                static_assert(std::is_pod<T>{});
        }

        tagged_empty() = default;

        template<class... Args>
        constexpr tagged_empty(Args&&...) noexcept {}
};

template<bool Condition, class Base>
struct conditional_base
:
        std::conditional_t<Condition, Base, tagged_empty<Base>>
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(!Condition || std::is_pod<Base>{});
        }
};

}       // namespace block_adl

using block_adl::conditional_base;

}       // namespace util
}       // namespace dctl
