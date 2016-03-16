#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT, XSTD_PP_TTI_TYPENAME
#include <tuple>                // make_tuple
#include <type_traits>          // bool_constant, conditional, false_type, true_type
#include <utility>              // forward

namespace dctl {

XSTD_PP_TTI_CONSTANT(initial_position_gap, 2)

XSTD_PP_TTI_CONSTANT(is_long_ranged_king, false)

using short_ranged_tag = std::false_type;
using  long_ranged_tag = std::true_type;

template<class Rules>
using king_range_category_t = std::conditional_t<
        is_long_ranged_king_v<Rules>,
         long_ranged_tag,
        short_ranged_tag
>;

XSTD_PP_TTI_CONSTANT(is_land_behind_piece, false)

template<class Rules>
using king_range_category_land_behind_piece_t = std::conditional_t<
        is_land_behind_piece_v<Rules>,
        short_ranged_tag,
        king_range_category_t<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_halt_behind_king, false)

template<class Rules>
using king_range_category_halt_behind_king_t = std::conditional_t<
        is_halt_behind_king_v<Rules>,
        short_ranged_tag,
        king_range_category_land_behind_piece_t<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)

using  forward_pawn_jump_tag = std::false_type;
using backward_pawn_jump_tag = std::true_type;

template<class Rules>
using pawn_jump_category_t = std::conditional_t<
        is_backward_pawn_jump_v<Rules>,
        backward_pawn_jump_tag,
         forward_pawn_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)

using   diagonal_jump_tag = std::false_type;
using orthogonal_jump_tag = std::true_type;

template<class Rules>
using jump_category_t = std::conditional_t<
        is_orthogonal_jump_v<Rules>,
        orthogonal_jump_tag,
          diagonal_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_superior_rank_jump, false)

XSTD_PP_TTI_CONSTANT(is_passing_promotion, false)

using stopped_promotion_tag = std::false_type;
using passing_promotion_tag = std::true_type;

template<class Rules>
using promotion_category_t = std::conditional_t<
        is_passing_promotion_v<Rules>,
        passing_promotion_tag,
        stopped_promotion_tag
>;

XSTD_PP_TTI_CONSTANT(is_passing_capture, false)

using stopped_capture_tag = std::false_type;
using passing_capture_tag = std::true_type;

template<class Rules>
using capture_category_t = std::conditional_t<
        is_passing_capture_v<Rules>,
        passing_capture_tag,
        stopped_capture_tag
>;

template<class Rules>
constexpr auto is_reversible_king_jump_direction_v =
        is_passing_capture_v<Rules> && is_long_ranged_king_v<Rules>
;

template<class Rules>
using is_reversible_king_jump_direction = std::bool_constant<
        is_reversible_king_jump_direction_v<Rules>>
;

template<class Rules>
constexpr auto large_jump_v =
        ((is_orthogonal_jump_v<Rules> && is_backward_pawn_jump_v<Rules>) ||
         (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>) ||
         (is_reversible_king_jump_direction_v<Rules> && is_backward_pawn_jump_v<Rules>)) ? 3 : 4
;

template<class Rules>
using large_jump = std::integral_constant<int, large_jump_v<Rules>>;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_passing_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reversible_king_jump_direction_v<Rules>))
;

template<class Rules>
using is_unambiguous_pawn_jump = std::bool_constant<
        is_unambiguous_pawn_jump_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_quantity_precedence, false)
XSTD_PP_TTI_CONSTANT(is_quality_precedence, false)
XSTD_PP_TTI_CONSTANT(is_modality_precedence, false)
XSTD_PP_TTI_CONSTANT(is_ordering_precedence, false)

struct empty_tuple
{
        template<class Action>
        constexpr auto operator()(Action const&) const noexcept
        {
                return std::make_tuple();
        }
};

XSTD_PP_TTI_TYPENAME(precedence_tuple, empty_tuple)

using    trivial_precedence_tag = std::false_type;
using nontrivial_precedence_tag = std::true_type;

template<class Rules>
using precedence_category_t = std::conditional_t<
        is_precedence_tuple_v<Rules>,
        nontrivial_precedence_tag,
           trivial_precedence_tag
>;

template<class Rules>
using is_trivial_precedence = std::is_same<
        precedence_category_t<Rules>,
        trivial_precedence_tag
>;

template<class Rules>
constexpr auto is_trivial_precedence_v = is_trivial_precedence<Rules>::value;

using keep_duplicates_tag = std::false_type;
using drop_duplicates_tag = std::true_type;

namespace precedence {

template<class Rules>
struct equal_to
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State const&... s) const noexcept
        {
                return
                        precedence_tuple_t<Rules>{}(std::forward<Action1>(a1), s...) ==
                        precedence_tuple_t<Rules>{}(std::forward<Action2>(a2), s...)
                ;
        }
};

template<class Rules>
struct less
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State const&... s) const noexcept
        {
                return
                        precedence_tuple_t<Rules>{}(std::forward<Action1>(a1), s...) <
                        precedence_tuple_t<Rules>{}(std::forward<Action2>(a2), s...)
                ;
        }
};

template<class Rules>
struct not_equal_to
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State&&... s) const noexcept
        {
                return !equal_to<Rules>{}(std::forward<Action1>(a1), std::forward<Action1>(a2), std::forward<State>(s)...);
        }
};

template<class Rules>
struct greater
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State&&... s) const noexcept
        {
                return less<Rules>{}(std::forward<Action1>(a2), std::forward<Action1>(a1), std::forward<State>(s)...);
        }
};

template<class Rules>
struct greater_equal
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State&&... s) const noexcept
        {
                return !less<Rules>{}(std::forward<Action1>(a1), std::forward<Action1>(a2), std::forward<State>(s)...);
        }
};

template<class Rules>
struct less_equal
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State const&... s) const noexcept
        {
                return !less<Rules>{}(std::forward<Action1>(a2), std::forward<Action1>(a1), s...);
        }
};

template<class Rules>
struct equivalent_to
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1 const& a1, Action2 const& a2, State const&... s) const noexcept
        {
                return !(less<Rules>{}(a1, a2, s...) || less<Rules>{}(a2, a1, s...));
        }
};

template<class Rules>
struct not_equivalent_to
{
        template<class Action1, class Action2, class... State>
        constexpr auto operator()(Action1&& a1, Action2&& a2, State&&... s) const noexcept
        {
                return !equivalent_to<Rules>{}(std::forward<Action1>(a1), std::forward<Action1>(a2), std::forward<State>(s)...);
        }
};

}       // namespace precedence

XSTD_PP_TTI_CONSTANT(max_same_king_push, 0)

template<class Rules>
constexpr auto is_restricted_king_push_v = max_same_king_push_v<Rules> != 0;

template<class Rules>
using is_restricted_king_push = std::bool_constant<
        is_restricted_king_push_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(max_reversible_moves, 0)

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

template<class Rules>
using is_restricted_reversible_moves = std::bool_constant<
        is_restricted_reversible_moves_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_algebraic_notation, false)

using   numeric_notation_tag = std::false_type;
using algebraic_notation_tag = std::true_type;

template<class Rules>
using notation_category_t = std::conditional_t<
        is_algebraic_notation_v<Rules>,
        algebraic_notation_tag,
          numeric_notation_tag
>;

XSTD_PP_TTI_CONSTANT(pushsep, '-')
XSTD_PP_TTI_CONSTANT(jumpsep, 'x')

}       // namespace dctl
