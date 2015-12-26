#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave/patterns.hpp>                 // PushTargets
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <type_traits>                                  // false_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        set_type const active_pawns;
        set_type const not_occupied;
public:
        Detect(set_type const& p, set_type const& e) noexcept
        :
                active_pawns{p},
                not_occupied{e}
        {}

        explicit Detect(State const& state) noexcept
        :
                active_pawns{state.pieces(ToMove, Piece::pawn)},
                not_occupied{state.not_occupied()}
        {}

        auto operator()() const noexcept
        {
                return active_pawns.any() ? directions_lfold<left_up, right_up>() : false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold() const noexcept
        {
                return (push_targets<Directions<orientation>{}>{}(active_pawns, not_occupied).any() || ...);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
