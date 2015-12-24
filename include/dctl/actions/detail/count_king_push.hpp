#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave.hpp>                          // PushTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <xstd/cstddef.hpp>                             // _z

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
        State const& state;

public:
        explicit Count(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return sources(state.pieces(ToMove, Piece::king));
        }

        auto operator()(set_type const& active_kings) const noexcept
        {
                return sources(active_kings);
        }

private:
        auto sources(set_type const& active_kings) const noexcept
        {
                using namespace xstd::support_literals;
                return active_kings.none() ?  0_z : directions_lfold<left_up, right_up, left_down, right_down>(active_kings);
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const& active_kings) const
        {
                return (targets<Directions<orientation>{}>(active_kings) + ...);
        }

        template<int Direction>
        auto targets(set_type const& active_kings) const noexcept
        {
                return push_targets<Direction>{}(
                        active_kings, state.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
