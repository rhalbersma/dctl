#pragma once
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave/patterns.hpp>                 // Targets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::king, select::jump, Reverse, State>
{
        using   board_type = board_t<State>;
        using   rules_type = rules_t<State>;
        using     set_type =   set_t<State>;

        template<int Direction>
        using jump_targets = JumpTargets<board_type, Direction, king_range_category_t<rules_type>>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        set_type const active_kings;
        set_type const king_targets;
        set_type const not_occupied;
public:
        Detect(set_type const& k, set_type const& t, set_type const& e) noexcept
        :
                active_kings{k},
                king_targets{t},
                not_occupied{e}
        {}

        explicit Detect(State const& state) noexcept
        :
                active_kings{state.pieces(ToMove, Piece::king)},
                king_targets{state.king_targets(!ToMove)},
                not_occupied{state.not_occupied()}
        {}

        auto operator()() const noexcept
        {
                return active_kings.any() ? directions_dispatch(jump_category_t<rules_type>{}) : false;
        }
private:
        auto directions_dispatch(diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up, left_down, right_down>();
        }

        auto directions_dispatch(orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>();
        }

        template<template<int> class... Directions>
        auto directions_lfold() const noexcept
        {
                return (jump_targets<Directions<orientation>{}>{}(active_kings, king_targets, not_occupied).any() || ...);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl