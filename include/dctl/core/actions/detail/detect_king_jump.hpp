#pragma once
#include <dctl/core/actions/detail/detect_primary_fwd.hpp>      // detect (primary template)
#include <dctl/core/actions/select/jump.hpp>                    // jump
#include <dctl/core/board/angle.hpp>                            // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/mask/jump_targets.hpp>                // jump_targets
#include <dctl/core/state/color_piece.hpp>                      // color, color_, king_
#include <dctl/core/rules/traits.hpp>                           // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/util/type_traits.hpp>                            // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<color Side, class Reverse, class State>
class detect<color_<Side>, kings_, select::jump, Reverse, State>
{
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = kings_c;
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = board::mask::jump_targets<board_type, Direction, king_range_category_t<rules_type>>;

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
                return (... || !jump_targets<Directions<orientation>{}>{}(sources, targets, destinations).empty());
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
