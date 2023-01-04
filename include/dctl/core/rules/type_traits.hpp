#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // dir_...
#include <dctl/util/tti.hpp>            // PP_TTI_STATIC_MEMBER
#include <dctl/util/type_traits.hpp>    // rules_t
#include <tabula/tuple.hpp>             // tuple_c
#include <algorithm>                    // min, max
#include <concepts>                     // same_as
#include <tuple>                        // make_tuple
#include <type_traits>                  // conditional_t, decay_t, false_type, true_type
#include <utility>                      // forward

namespace dctl::core {

PP_TTI_STATIC_MEMBER(width, 8)
PP_TTI_STATIC_MEMBER(height, 8)
PP_TTI_STATIC_MEMBER(coloring, 1)
PP_TTI_STATIC_MEMBER(is_orthogonal_jumps, false)

template<class Geometry>
inline constexpr auto is_placeable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 1 && (coloring_v<Geometry> || std::max(width_v<Geometry>, height_v<Geometry>) > 1);

template<class Geometry>
inline constexpr auto is_pushable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 2;

template<class Geometry>
inline constexpr auto is_jumpable_v = std::min(width_v<Geometry>, height_v<Geometry>) >= 3 && (coloring_v<Geometry> || std::max(width_v<Geometry>, height_v<Geometry>) > 3);

PP_TTI_STATIC_MEMBER(initial_position_gap, 2)

PP_TTI_STATIC_MEMBER(is_long_ranged_king, false)

struct short_ranged_tag : std::false_type {};
struct  long_ranged_tag : std:: true_type {};

template<class Rules>
using king_range_category_t = std::conditional_t<
        is_long_ranged_king_v<Rules>,
         long_ranged_tag,
        short_ranged_tag
>;

PP_TTI_STATIC_MEMBER(is_land_behind_piece, false)
PP_TTI_STATIC_MEMBER(is_halt_behind_king, false)
PP_TTI_STATIC_MEMBER(is_backward_pawn_jump, false)
PP_TTI_STATIC_MEMBER(is_superior_rank_jump, false)
PP_TTI_STATIC_MEMBER(is_passing_promotion, false)
PP_TTI_STATIC_MEMBER(is_passing_capture, false)

template<class Rules>
inline constexpr auto is_reverse_king_jump_v = is_long_ranged_king_v<Rules> && is_passing_capture_v<Rules>;

template<class Rules>
inline constexpr auto large_jump_v =
        ((is_orthogonal_jumps_v<Rules> && is_backward_pawn_jump_v<Rules>) ||
         (is_orthogonal_jumps_v<Rules> && is_reverse_king_jump_v<Rules>) ||
         (is_reverse_king_jump_v<Rules> && is_backward_pawn_jump_v<Rules>)) ? 3 : 4
;

template<class Rules>
inline constexpr auto is_unambiguous_pawn_jump_v =
        !(is_backward_pawn_jump_v<Rules> || is_passing_promotion_v<Rules> ||
        (is_orthogonal_jumps_v<Rules> && is_reverse_king_jump_v<Rules>))
;

PP_TTI_STATIC_MEMBER(is_quantity_precedence, false)
PP_TTI_STATIC_MEMBER(is_contents_precedence, false)
PP_TTI_STATIC_MEMBER(is_modality_precedence, false)
PP_TTI_STATIC_MEMBER(is_ordering_precedence, false)

inline constexpr auto trivial_precedence_c = [](auto&&) {
        return std::make_tuple();
};

PP_TTI_STATIC_MEMBER(precedence, trivial_precedence_c)

template<class Rules>
inline constexpr auto is_trivial_precedence_v = std::same_as<
        decltype(precedence_v<Rules>),
        decltype(trivial_precedence_c)
>;

template<class Action, class Rules = rules_t<Action>, auto Order = precedence_v<Rules>>
struct precedence
{
        Action const& m_action;

        constexpr auto operator<=>(precedence const& other) const noexcept
        {
                return
                        Order(this->m_action) <=>
                        Order(other.m_action)
                ;
        }
};

template<class Action>
precedence(Action) -> precedence<Action, rules_t<Action>, precedence_v<rules_t<Action>>>;

inline constexpr auto to_precedence = [](auto&& action) {
        return precedence{std::forward<decltype(action)>(action)};
};

struct keep_duplicates_tag : std::false_type {};
struct drop_duplicates_tag : std:: true_type {};

PP_TTI_STATIC_MEMBER(max_same_king_move, 0)

template<class Rules>
inline constexpr auto is_restricted_king_move_v = max_same_king_move_v<Rules> != 0;

PP_TTI_STATIC_MEMBER(max_reversible_moves, 0)

template<class Rules>
inline constexpr auto is_restricted_reversible_moves_v = max_reversible_moves_v<Rules> != 0;

PP_TTI_STATIC_MEMBER(pushsep, '-')
PP_TTI_STATIC_MEMBER(jumpsep, 'x')

PP_TTI_STATIC_MEMBER(is_algebraic_notation, false)

enum class notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric = 2
};

template<class Rules>
inline constexpr auto notation_v =
        is_algebraic_notation_v<Rules> ?
        notation::algebraic :
        notation::numeric
;

PP_TTI_STATIC_MEMBER(pawn_move_directions, (tabula::tuple_c<int, dir_NE, dir_NW>))
PP_TTI_STATIC_MEMBER(pawn_jump_directions, (tabula::tuple_c<int, dir_NE, dir_NW>))
PP_TTI_STATIC_MEMBER(king_move_directions, (tabula::tuple_c<int, dir_NE, dir_NW, dir_SW, dir_SE>))
PP_TTI_STATIC_MEMBER(king_jump_directions, (tabula::tuple_c<int, dir_NE, dir_NW, dir_SW, dir_SE>))

}       // namespace dctl::core
