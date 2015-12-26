#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <dctl/board/wave/patterns.hpp>                 // PushTargets
#include <type_traits>                                  // false_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::king, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        set_type const active_kings;
        set_type const not_occupied;
public:
        explicit Detect(State const& state) noexcept
        :
                active_kings{state.pieces(ToMove, Piece::king)},
                not_occupied{state.not_occupied()}
        {}

        auto operator()() const noexcept
        {
                return active_kings.any() ? directions_lfold<left_up, right_up, left_down, right_down>() : false;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold() const noexcept
        {
                return (push_targets<Directions<orientation>{}>{}(active_kings, not_occupied).any() || ...);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
