#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT, XSTD_PP_TTI_TYPENAME
#include <type_traits>          // integral_constant

namespace dctl {
namespace precedence {
namespace trivial {

struct equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const&, Action2 const&) const noexcept
        {
                return true;
        }
};

struct less
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const&, Action2 const&) const noexcept
        {
                return false;
        }
};

}       // namespace trivial

XSTD_PP_TTI_TYPENAME(equal_to, trivial::equal_to)
XSTD_PP_TTI_TYPENAME(less    , trivial::less    )

template<class Rules>
struct not_equal_to_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !equal_to_t<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct greater_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return less_t<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct greater_equal_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !less_t<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
struct less_equal_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !less_t<Rules>{}(rhs, lhs);
        }
};

template<class Rules>
struct equivalent_to_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !(less_t<Rules>{}(lhs, rhs) || less_t<Rules>{}(rhs, lhs));
        }
};

template<class Rules>
struct not_equivalent_to_t
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
        {
                return !equivalent_to_t<Rules>{}(lhs, rhs);
        }
};

template<class Rules>
constexpr auto is_complete_v = !(is_equal_to_v<Rules> ^ is_less_v<Rules>);

template<class Rules>
using is_complete_t = std::bool_constant<is_complete_v<Rules>>;

template<class Rules>
constexpr auto is_trivial_v = !(is_equal_to_v<Rules> || is_less_v<Rules>);

template<class Rules>
using is_trivial_t = std::bool_constant<is_trivial_v<Rules>>;

XSTD_PP_TTI_CONSTANT(is_quantity, false)
XSTD_PP_TTI_CONSTANT(is_quality, false)
XSTD_PP_TTI_CONSTANT(is_modality, false)
XSTD_PP_TTI_CONSTANT(is_ordering, false)

}       // namespace precedence
}       // namespace dctl
