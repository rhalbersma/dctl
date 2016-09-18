#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular
#include <dctl/utility/type_traits.hpp> // rules_t
#include <xstd/pp/tti.hpp>              // XSTD_PP_TTI_CONSTANT, XSTD_PP_TTI_TYPENAME
#include <xstd/tuple.hpp>               // compare
#include <tuple>                        // make_tuple
#include <type_traits>                  // bool_constant, conditional, decay, is_same, false_type, true_type
#include <utility>                      // forward

namespace dctl {

XSTD_PP_TTI_CONSTANT(width, 8)
XSTD_PP_TTI_CONSTANT(height, 8)
XSTD_PP_TTI_CONSTANT(is_inverted, false)

XSTD_PP_TTI_CONSTANT(initial_position_gap, 2)

XSTD_PP_TTI_CONSTANT(is_long_ranged_king, false)

struct short_ranged_tag : std::false_type {};
struct  long_ranged_tag : std:: true_type {};

template<class Rules>
using king_range_category_t = std::conditional_t<
        is_long_ranged_king_or_v<Rules>,
         long_ranged_tag,
        short_ranged_tag
>;

XSTD_PP_TTI_CONSTANT(is_land_behind_piece, false)

template<class Rules>
constexpr auto is_land_behind_piece_v =
        !is_long_ranged_king_or_v<Rules> || is_land_behind_piece_or_v<Rules>
;

template<class Rules>
using king_range_category_land_behind_piece_t = std::conditional_t<
        is_land_behind_piece_v<Rules>,
        short_ranged_tag,
         long_ranged_tag
>;

XSTD_PP_TTI_CONSTANT(is_halt_behind_king, false)

template<class Rules>
constexpr auto is_halt_behind_king_v =
        is_land_behind_piece_v<Rules> || is_halt_behind_king_or_v<Rules>
;

template<class Rules>
using king_range_category_halt_behind_king_t = std::conditional_t<
        is_halt_behind_king_v<Rules>,
        short_ranged_tag,
         long_ranged_tag
>;

XSTD_PP_TTI_CONSTANT(is_backward_pawn_jump, false)

struct  forward_pawn_jump_tag : std::false_type {};
struct backward_pawn_jump_tag : std:: true_type {};

template<class Rules>
using pawn_jump_category_t = std::conditional_t<
        is_backward_pawn_jump_or_v<Rules>,
        backward_pawn_jump_tag,
         forward_pawn_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_orthogonal_jump, false)

struct   diagonal_jump_tag : std::false_type {};
struct orthogonal_jump_tag : std:: true_type {};

template<class Rules>
using jump_category_t = std::conditional_t<
        is_orthogonal_jump_or_v<Rules>,
        orthogonal_jump_tag,
          diagonal_jump_tag
>;

template<class Rules>
using rectangular_t = board::Rectangular<
        width_or_v<Rules>,
        height_or_v<Rules>,
        is_inverted_or_v<Rules>,
        is_orthogonal_jump_or_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_superior_rank_jump, false)

struct inferior_rank_jump_tag : std::false_type {};
struct superior_rank_jump_tag : std:: true_type {};

template<class Rules>
using rank_jump_category_t = std::conditional_t<
        is_superior_rank_jump_or_v<Rules>,
        superior_rank_jump_tag,
        inferior_rank_jump_tag
>;

XSTD_PP_TTI_CONSTANT(is_passing_promotion, false)

struct stopped_promotion_tag : std::false_type {};
struct passing_promotion_tag : std:: true_type {};

template<class Rules>
using promotion_category_t = std::conditional_t<
        is_passing_promotion_or_v<Rules>,
        passing_promotion_tag,
        stopped_promotion_tag
>;

XSTD_PP_TTI_CONSTANT(is_passing_capture, false)

struct stopped_capture_tag : std::false_type {};
struct passing_capture_tag : std:: true_type {};

template<class Rules>
using capture_category_t = std::conditional_t<
        is_passing_capture_or_v<Rules>,
        passing_capture_tag,
        stopped_capture_tag
>;

template<class Rules>
constexpr auto is_reverse_king_jump_v =
        is_passing_capture_or_v<Rules> && is_long_ranged_king_or_v<Rules>
;

struct forward_king_jump_tag : std::false_type {};
struct reverse_king_jump_tag : std:: true_type {};

template<class Rules>
using king_jump_category_t = std::conditional_t<
        is_reverse_king_jump_v<Rules>,
        reverse_king_jump_tag,
        forward_king_jump_tag
>;

template<class Rules>
constexpr auto large_jump_v =
        ((is_orthogonal_jump_or_v<Rules> && is_backward_pawn_jump_or_v<Rules>) ||
         (is_orthogonal_jump_or_v<Rules> && is_reverse_king_jump_v<Rules>) ||
         (is_reverse_king_jump_v<Rules> && is_backward_pawn_jump_or_v<Rules>)) ? 3 : 4
;

template<class Rules>
using large_jump = std::integral_constant<int, large_jump_v<Rules>>;

template<class Rules>
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_or_v<Rules> || is_passing_promotion_or_v<Rules> ||
        (is_orthogonal_jump_or_v<Rules> && is_reverse_king_jump_v<Rules>))
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
        constexpr auto operator()(Action&&) const noexcept
        {
                return std::make_tuple();
        }
};

XSTD_PP_TTI_TYPENAME(tuple, empty_tuple)

template<class Rules>
constexpr auto is_nontrivial_precedence_v = is_tuple_v<Rules>;

struct    trivial_precedence_tag : std::false_type {};
struct nontrivial_precedence_tag : std:: true_type {};

template<class Rules>
using precedence_category_t = std::conditional_t<
        is_nontrivial_precedence_v<Rules>,
        nontrivial_precedence_tag,
           trivial_precedence_tag
>;

struct keep_duplicates_tag : std::false_type {};
struct drop_duplicates_tag : std:: true_type {};

namespace precedence {

struct compare
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                using rules_type1 = rules_t<std::decay_t<Action1>>;
                using rules_type2 = rules_t<std::decay_t<Action2>>;
                static_assert(std::is_same<rules_type1, rules_type2>{});
                return xstd::compare(
                        tuple_or_t<rules_type1>{}(std::forward<Action1>(a1)),
                        tuple_or_t<rules_type2>{}(std::forward<Action2>(a2))
                );
        }
};

struct equal_to
{
        template<class Action1, class Action2>
        constexpr auto operator()(Action1&& a1, Action2&& a2) const noexcept
        {
                using rules_type1 = rules_t<std::decay_t<Action1>>;
                using rules_type2 = rules_t<std::decay_t<Action2>>;
                static_assert(std::is_same<rules_type1, rules_type2>{});
                return
                        tuple_or_t<rules_type1>{}(std::forward<Action1>(a1)) ==
                        tuple_or_t<rules_type2>{}(std::forward<Action2>(a2))
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
                static_assert(std::is_same<rules_type1, rules_type2>{});
                return
                        tuple_or_t<rules_type1>{}(std::forward<Action1>(a1)) <
                        tuple_or_t<rules_type2>{}(std::forward<Action2>(a2))
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

XSTD_PP_TTI_CONSTANT(max_same_king_push, 0)

template<class Rules>
constexpr auto is_restricted_king_push_v = max_same_king_push_or_v<Rules> != 0;

template<class Rules>
using is_restricted_king_push = std::bool_constant<
        is_restricted_king_push_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(max_reversible_moves, 0)

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_or_v<Rules> != 0;

template<class Rules>
using is_restricted_reversible_moves = std::bool_constant<
        is_restricted_reversible_moves_v<Rules>
>;

XSTD_PP_TTI_CONSTANT(is_algebraic_notation, false)

struct   numeric_notation_tag : std::false_type {};
struct algebraic_notation_tag : std:: true_type {};

template<class Rules>
using notation_category_t = std::conditional_t<
        is_algebraic_notation_or_v<Rules>,
        algebraic_notation_tag,
          numeric_notation_tag
>;

XSTD_PP_TTI_CONSTANT(pushsep, '-')
XSTD_PP_TTI_CONSTANT(jumpsep, 'x')

}       // namespace dctl
