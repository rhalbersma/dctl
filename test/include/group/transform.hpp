#pragma once

namespace dctl {
namespace group {

template<class Function, class Object>
constexpr Object apply(Function fun, Object const& obj, int n) noexcept
{
        return n ? fun(apply(fun, obj, n - 1)) : obj;
}

struct IsIdentity
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 1) == apply(fun, obj, 0);
        }
};

struct IsInvolution
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 2) == apply(fun, obj, 0);
        }
};

struct IsIdempotent
{
        template<class Function, class Object>
        constexpr auto operator()(Function fun, Object const& obj) const noexcept
        {
                return apply(fun, obj, 2) == apply(fun, obj, 1);
        }
};

}       // namespace group
}       // namespace dctl
