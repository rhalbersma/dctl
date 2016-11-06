#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/color_piece.hpp>                         // color, color_, king_
#include <dctl/rule_traits.hpp>                         // king_range_category
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace detail {

template<color Side, class Reverse, class State>
class count<color_<Side>, king_, select::push, Reverse, State>
{
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = kings_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using king_push_targets = board::mask::push_targets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.value();

public:
        auto operator()(State const& s) const noexcept
                -> std::size_t
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); sources.any()) {
                        return directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(sources, s.pieces(empty_c));
                }
                return 0;
        }

private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const destinations) const noexcept
        {
                return (... + king_push_targets<Directions<orientation>{}>{}(sources, destinations).count());
        }
};

}       // namespace detail
}       // namespace dctl
