#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/wave.hpp>                          // PushTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/rule_traits.hpp>                         // short_ranged_tag
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Count<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
        State const& state;
public:
        explicit Count(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return pieces<ToMove, Piece::pawn>(state).any() ? directions_lfold<left_up, right_up>() : 0;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold() const noexcept
        {
                return (... + push_targets<Directions<bearing.degrees()>{}>{}(pieces<ToMove, Piece::pawn>(state), state.not_occupied()).count());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
