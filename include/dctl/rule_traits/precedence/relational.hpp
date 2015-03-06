#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_TYPE

namespace dctl {
namespace precedence {
namespace trivial {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& /* lhs */, Move const& /* rhs */) noexcept
        {
                return true;
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& /* lhs */, Move const& /* rhs */) noexcept
        {
                return false;
        }
};

}       // namespace trivial

XSTD_PP_TTI_TYPE(equal_to, trivial::equal_to)
XSTD_PP_TTI_TYPE(less    , trivial::less    )

template<class Rules>
struct not_equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !equal_to<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct greater
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return less<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct greater_equal
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !less<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct less_equal
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !less<Rules>{}(rhs, lhs);
        }
};

}       // namespace precedence
}       // namespace dctl
