#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace group {

template<class Function, class Object>
constexpr auto apply(Function fun, Object const& obj, std::size_t const n) noexcept
        -> Object
{
        return (n == 0) ? obj : fun(apply(fun, obj, n - 1));
}

struct is_identity
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 1) == apply(fun, obj, 0);
        }
};

struct is_involution
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 2) == apply(fun, obj, 0);
        }
};

struct is_idempotent
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 2) == apply(fun, obj, 1);
        }
};

}       // namespace group
}       // namespace dctl
