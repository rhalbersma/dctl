#pragma once
#include <type_traits>  // conditional, is_empty, is_pod

namespace dctl {
namespace util {
namespace block_adl {

template<class Tag>
struct tagged_empty
{
        constexpr static auto static_assert_type_traits() noexcept
        {
                using T = tagged_empty<Tag>;
                static_assert(std::is_empty_v<T>);
                static_assert(std::is_pod_v<T>);
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
        constexpr static auto static_assert_type_traits() noexcept
        {
                static_assert(not Condition || std::is_pod_v<Base>);
        }
};

}       // namespace block_adl

using block_adl::conditional_base;

}       // namespace util
}       // namespace dctl
