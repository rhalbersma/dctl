#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/jump_targets.hpp>             // jump_targets
#include <dctl/color_piece.hpp>                         // color, color_, pawn_
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump, is_orthogonal_jump
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <type_traits>                                  // is_same

namespace dctl {
namespace detail {

template<color Side, class Reverse, class State>
class detect<color_<Side>, pawns_, select::jump, Reverse, State>
{
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = pawns_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = board::mask::jump_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.value();
public:
        auto operator()(State const& s) const noexcept
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); !sources.empty()) {
                        return directions(sources, s.targets(to_move_c, piece_c), s.pieces(empty_c));
                }
                return false;
        }
private:
        auto directions(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right_up, board::left_up>(
                                sources, targets, destinations
                        );
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(
                                sources, targets, destinations
                        );
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left>(
                                sources, targets, destinations
                        );
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left, board::left_down, board::down, board::right_down>(
                                sources, targets, destinations
                        );
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                return (... || !jump_targets<Directions<orientation>{}>{}(sources, targets, destinations).empty());
        }
};

}       // namespace detail
}       // namespace dctl
