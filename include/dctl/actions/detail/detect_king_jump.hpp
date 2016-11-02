#pragma once
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/jump_targets.hpp>             // jump_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, king_
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State>
class Detect<color_constant<Side>, king_, select::jump, Reverse, State>
{
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = king_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = board::mask::jump_targets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.degrees();
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const sources = state.pieces(to_move_c, piece_c); sources.any()) {
                        return directions(sources, state.targets(to_move_c, piece_c), state.pieces(none_c));
                }
                return false;
        }
private:
        auto directions(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                if constexpr (is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left, board::left_down, board::down, board::right_down>(
                                sources, targets, destinations
                        );
                } else {
                        return directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(
                                sources, targets, destinations
                        );
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const targets, set_type const destinations) const noexcept
        {
                return (... || jump_targets<Directions<orientation>{}>{}(sources, targets, destinations).any());
        }
};

}       // namespace detail
}       // namespace dctl
