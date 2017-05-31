#pragma once
#include <dctl/core/board/rectangular.hpp>   // rectangular
#include <dctl/util/tti.hpp>         // DCTL_PP_TTI_CONSTANT
#include <dctl/util/type_traits.hpp> // rules_t
#include <tuple>                        // make_tuple
#include <type_traits>                  // bool_constant, conditional, decay, is_same, false_type, true_type
#include <utility>                      // forward

namespace dctl {

DCTL_PP_TTI_CONSTANT(width, 8)
DCTL_PP_TTI_CONSTANT(height, 8)
DCTL_PP_TTI_CONSTANT(is_inverted, false)
DCTL_PP_TTI_CONSTANT(is_orthogonal_jump, false)

template<class Rules>
using rectangular_t = board::rectangular<
        width_v<Rules>,
        height_v<Rules>,
        is_inverted_v<Rules>,
        is_orthogonal_jump_v<Rules>
>;

DCTL_PP_TTI_CONSTANT(initial_position_gap, 2)

DCTL_PP_TTI_CONSTANT(is_long_ranged_king, false)

struct short_ranged_tag : std::false_type {};
struct  long_ranged_tag : std:: true_type {};

template<class Rules>
using king_range_category_t = std::conditional_t<
        is_long_ranged_king_v<Rules>,
         long_ranged_tag,
        short_ranged_tag
>;

DCTL_PP_TTI_CONSTANT(is_land_behind_piece, false)
DCTL_PP_TTI_CONSTANT(is_halt_behind_king, false)

DCTL_PP_TTI_CONSTANT(is_backward_pawn_jump, false)

DCTL_PP_TTI_CONSTANT(is_superior_rank_jump, false)

DCTL_PP_TTI_CONSTANT(is_passing_promotion, false)
struct passing_promotion_tag {};

DCTL_PP_TTI_CONSTANT(is_passing_capture, false)

template<class Rules>
constexpr auto is_reverse_king_jump_v = is_long_ranged_king_v<Rules> && is_passing_capture_v<Rules>;

template<class Rules>
constexpr auto large_jump_v =
        ((is_orthogonal_jump_v<Rules> && is_backward_pawn_jump_v<Rules>) ||
         (is_orthogonal_jump_v<Rules> && is_reverse_king_jump_v<Rules>) ||
         (is_reverse_king_jump_v<Rules> && is_backward_pawn_jump_v<Rules>)) ? 3 : 4
;

template<class Rules>
using large_jump = std::integral_constant<int, large_jump_v<Rules>>;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_passing_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reverse_king_jump_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump = std::bool_constant<
        is_unambiguous_pawn_jump_v<Rules>
>;

DCTL_PP_TTI_CONSTANT(is_quantity_precedence, false)
DCTL_PP_TTI_CONSTANT(is_contents_precedence, false)
DCTL_PP_TTI_CONSTANT(is_modality_precedence, false)
DCTL_PP_TTI_CONSTANT(is_ordering_precedence, false)

inline constexpr auto trivial_precedence_c = [](auto&&) {
        return std::make_tuple();
};

DCTL_PP_TTI_CONSTANT(precedence, trivial_precedence_c)

template<class Rules>
constexpr auto is_trivial_precedence_v = std::is_same<
        decltype(precedence_v<Rules>),
        decltype(trivial_precedence_c)
>::value;

namespace precedence {

struct equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                using rules_type1 = rules_t<std::decay_t<Action1>>;
                using rules_type2 = rules_t<std::decay_t<Action2>>;
                static_assert(std::is_same_v<rules_type1, rules_type2>);
                return
                        precedence_v<rules_type1>(std::forward<Action1>(a1)) ==
                        precedence_v<rules_type2>(std::forward<Action2>(a2))
                ;
        }
};

struct less
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                using rules_type1 = rules_t<std::decay_t<Action1>>;
                using rules_type2 = rules_t<std::decay_t<Action2>>;
                static_assert(std::is_same_v<rules_type1, rules_type2>);
                return
                        precedence_v<rules_type1>(std::forward<Action1>(a1)) <
                        precedence_v<rules_type2>(std::forward<Action2>(a2))
                ;
        }
};

struct not_equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                return !equal_to{}(std::forward<Action1>(a1), std::forward<Action1>(a2));
        }
};

struct greater
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                return less{}(std::forward<Action1>(a2), std::forward<Action1>(a1));
        }
};

struct greater_equal
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                return !less{}(std::forward<Action1>(a1), std::forward<Action1>(a2));
        }
};

struct less_equal
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                return !less{}(std::forward<Action1>(a2), std::forward<Action1>(a1));
        }
};

struct equivalent_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1 const& a1, Action2 const& a2) const noexcept
        {
                return !(less{}(a1, a2) || less{}(a2, a1));
        }
};

struct not_equivalent_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                return !equivalent_to{}(std::forward<Action1>(a1), std::forward<Action1>(a2));
        }
};

}       // namespace precedence

struct keep_duplicates_tag : std::false_type {};
struct drop_duplicates_tag : std:: true_type {};

DCTL_PP_TTI_CONSTANT(max_same_king_push, 0)

template<class Rules>
constexpr auto is_restricted_king_push_v = max_same_king_push_v<Rules> != 0;

template<class Rules>
using is_restricted_king_push = std::bool_constant<
        is_restricted_king_push_v<Rules>
>;

DCTL_PP_TTI_CONSTANT(max_reversible_moves, 0)

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

template<class Rules>
using is_restricted_reversible_moves = std::bool_constant<
        is_restricted_reversible_moves_v<Rules>
>;

DCTL_PP_TTI_CONSTANT(is_algebraic_notation, false)
DCTL_PP_TTI_CONSTANT(pushsep, '-')
DCTL_PP_TTI_CONSTANT(jumpsep, 'x')

}       // namespace dctl
