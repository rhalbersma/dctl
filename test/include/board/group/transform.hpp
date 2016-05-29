#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace group {
namespace detail {

template<class UnaryFunction, class Arg>
constexpr auto iterate(UnaryFunction fun, Arg const& arg, std::size_t const n) noexcept
        -> Arg
{
        return (n == 0) ? arg : fun(iterate(fun, arg, n - 1));
}

}       // namespace detail

struct is_identity
{
        template<class UnaryFunction, class Arg>
        constexpr auto operator()(UnaryFunction fun, Arg const& arg) const noexcept
        {
                return detail::iterate(fun, arg, 1) == detail::iterate(fun, arg, 0);
        }
};

struct is_involution
{
        template<class UnaryFunction, class Arg>
        constexpr auto operator()(UnaryFunction fun, Arg const& arg) const noexcept
        {
                return detail::iterate(fun, arg, 2) == detail::iterate(fun, arg, 0);
        }
};

struct is_idempotent
{
        template<class UnaryFunction, class Arg>
        constexpr auto operator()(UnaryFunction fun, Arg const& arg) const noexcept
        {
                return detail::iterate(fun, arg, 2) == detail::iterate(fun, arg, 1);
        }
};

}       // namespace group
}       // namespace dctl
