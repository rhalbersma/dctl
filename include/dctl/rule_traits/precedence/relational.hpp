#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_TYPE

namespace dctl {
namespace precedence {

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

XSTD_PP_TTI_TYPE(equal_to, equal_to)
XSTD_PP_TTI_TYPE(less    , less    )

template<class Rules>
struct not_equal_to_t
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !equal_to_t<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct greater_t
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return less_t<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct greater_equal_t
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !less_t<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct less_equal_t
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return !less_t<Rules>{}(rhs, lhs);
        }
};

}       // namespace precedence
}       // namespace dctl
