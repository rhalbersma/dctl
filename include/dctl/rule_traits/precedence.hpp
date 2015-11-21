#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT, XSTD_PP_TTI_TYPENAME
#include <tuple>
#include <type_traits>          // bool_constant (C++1z)

namespace dctl {
namespace precedence {
namespace trivial {

struct precedence
{
        template<class Action>
        constexpr auto operator()(Action const&) const noexcept
        {
                return std::make_tuple();
        }
};

}       // namespace trivial

XSTD_PP_TTI_TYPENAME(precedence, trivial::precedence)

template<class Rules>
constexpr auto is_trivial_v = !is_precedence_v<Rules>;

template<class Rules>
using is_trivial_t = std::bool_constant<is_trivial_v<Rules>>;

template<class Rules>
struct equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return precedence_t<Rules>{}(lhs) == precedence_t<Rules>{}(rhs);
        }
};

template<class Rules>
struct less
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return precedence_t<Rules>{}(lhs) < precedence_t<Rules>{}(rhs);
        }
};

template<class Rules>
struct not_equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !equal_to<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct greater
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return less<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct greater_equal
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !less<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct less_equal
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !less<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct equivalent_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !(less<Rules>{}(lhs, rhs) || less<Rules>{}(rhs, lhs));
        }
};

template<class Rules>
struct not_equivalent_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !equivalent_to<Rules>{}(lhs, rhs);
        }
};

XSTD_PP_TTI_CONSTANT(is_quantity, false)
XSTD_PP_TTI_CONSTANT(is_quality, false)
XSTD_PP_TTI_CONSTANT(is_modality, false)
XSTD_PP_TTI_CONSTANT(is_ordering, false)

}       // namespace precedence
}       // namespace dctl
