#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>
#include <dctl/core/board/type_traits.hpp>      // is_orthogonal_jump_v
#include <dctl/util/meta.hpp>
#include <dctl/util/tti.hpp>                    // DCTL_PP_TTI_CONSTANT
#include <dctl/util/type_traits.hpp>            // rules_t
#include <tuple>                                // make_tuple
#include <type_traits>                          // conditional_t, decay_t, is_same_v, false_type, true_type
#include <utility>                              // forward

namespace dctl::core {

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
constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_passing_promotion_v<Rules> ||
        (is_orthogonal_jump_v<Rules> && is_reverse_king_jump_v<Rules>))
;

DCTL_PP_TTI_CONSTANT(is_quantity_precedence, false)
DCTL_PP_TTI_CONSTANT(is_contents_precedence, false)
DCTL_PP_TTI_CONSTANT(is_modality_precedence, false)
DCTL_PP_TTI_CONSTANT(is_ordering_precedence, false)

constexpr auto trivial_precedence_c = [](auto&&) {
        return std::make_tuple();
};

DCTL_PP_TTI_CONSTANT(precedence, trivial_precedence_c)

template<class Rules>
constexpr auto is_trivial_precedence_v = std::is_same_v<
        decltype(precedence_v<Rules>),
        decltype(trivial_precedence_c)
>;

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

DCTL_PP_TTI_CONSTANT(max_same_king_move, 0)

template<class Rules>
constexpr auto is_restricted_king_move_v = max_same_king_move_v<Rules> != 0;

DCTL_PP_TTI_CONSTANT(max_reversible_moves, 0)

template<class Rules>
constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

DCTL_PP_TTI_CONSTANT(pushsep, '-')
DCTL_PP_TTI_CONSTANT(jumpsep, 'x')

DCTL_PP_TTI_CONSTANT(is_algebraic_notation, false)

enum class notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric = 2
};

template<class Rules>
constexpr auto notation_v =
        is_algebraic_notation_v<Rules> ?
        notation::algebraic :
        notation::numeric
;

template<int A, int B>
constexpr auto rotate_v = rotate(angle{A}, angle{B}).value();

template<int Orientation>
struct rotated
{
        template<class Arg>
        struct apply
        :
                meta::integral_c<int, rotate_v<Orientation, Arg{}>>
        {};
};

using basic_pawn_move_directions = meta::list_c<int, dir_NE, dir_NW>;
using king_move_directions = meta::list_c<int, dir_NE, dir_NW, dir_SW, dir_SE>;

template<class Rules>
using basic_pawn_jump_directions = meta::switch_<
        meta::list_c<bool, is_backward_pawn_jump_v<Rules>, is_orthogonal_jump_v<Rules>>,
        meta::case_<meta::list_c<bool, true , true >, meta::list_c<int, dir_E, dir_NE, dir_N, dir_NW, dir_W, dir_SW, dir_S, dir_SE>>,
        meta::case_<meta::list_c<bool, false, true >, meta::list_c<int, dir_E, dir_NE, dir_N, dir_NW, dir_W                       >>,
        meta::case_<meta::list_c<bool, true , false>, meta::list_c<int,        dir_NE,        dir_NW,        dir_SW,        dir_SE>>,
        meta::case_<meta::list_c<bool, false, false>, meta::list_c<int,        dir_NE,        dir_NW                              >>
>;

template<class Rules>
using basic_king_jump_directions = std::conditional_t<
        is_orthogonal_jump_v<Rules>,
        meta::list_c<int, dir_E, dir_NE, dir_N, dir_NW, dir_W, dir_SW, dir_S, dir_SE>,
        meta::list_c<int,        dir_NE,        dir_NW,        dir_SW,        dir_SE>
>;

}       // namespace dctl::core
