#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave.hpp>                          // PushTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <xstd/cstddef.hpp>                             // _z
#include <type_traits>                                  // false_type

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

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        State const& state;

public:
        explicit Count(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return sources(state.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const noexcept
        {
                return sources(active_pawns);
        }

private:
        auto sources(set_type const& active_pawns) const noexcept
        {
                using namespace xstd::support_literals;
                return active_pawns.any() ? directions_lfold<left_up, right_up>(active_pawns) : 0_z;
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const& active_pawns) const
        {
                return (targets<Directions<orientation>{}>(active_pawns) + ...);
        }

        template<int Direction>
        auto targets(set_type const& active_pawns) const noexcept
        {
                return push_targets<Direction>{}(
                        active_pawns, state.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
