#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave.hpp>                          // PushTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // king_range_category
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Count<ToMove, Piece::king, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        set_type const active_kings;
        set_type const not_occupied;
public:
        Count(set_type const& k, set_type const& e) noexcept
        :
                active_kings{k},
                not_occupied{e}
        {}

        explicit Count(State const& state) noexcept
        :
                active_kings{state.pieces(ToMove, Piece::king)},
                not_occupied{state.not_occupied()}
        {}

        auto operator()() const noexcept
        {
                return active_kings.any() ? directions_lfold<left_up, right_up, left_down, right_down>() : 0;
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold() const noexcept
        {
                return (push_targets<Directions<orientation>{}>{}(active_kings, not_occupied).count() + ...);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
