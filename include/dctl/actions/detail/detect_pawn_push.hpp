#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/patterns.hpp>                      // push_targets
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/state/pieces.hpp>
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = board::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
public:
        auto operator()(State const& state) const noexcept
        {
                auto const active_pawns = pieces<ToMove, Piece::pawn>(state);
                return active_pawns.any() ? directions_lfold<left_up, right_up>(active_pawns, state.not_occupied()) : false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_pawns, set_type const not_occupied) const noexcept
        {
                return (... || push_targets<Directions<bearing.degrees>{}>{}(active_pawns, not_occupied).any());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
