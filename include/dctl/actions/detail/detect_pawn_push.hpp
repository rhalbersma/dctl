#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, pawn_
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State>
class Detect<color_constant<Side>, pawn_, select::push, Reverse, State>
{
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = pawn_c;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using pawn_push_targets = board::mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.degrees();
public:
        auto operator()(State const& state) const noexcept
        {
                if (auto const sources = state.pieces(to_move_c, piece_c); sources.any()) {
                        return directions_lfold<board::right_up, board::left_up>(sources, state.pieces(none_c));
                }
                return false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const destinations) const noexcept
        {
                return (... || pawn_push_targets<Directions<orientation>{}>{}(sources, destinations).any());
        }
};

}       // namespace detail
}       // namespace dctl
