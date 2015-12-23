#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave.hpp>                          // Sink
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

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        State const& state;

public:
        explicit Count(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return count(state.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const noexcept
        {
                return count(active_pawns);
        }

private:
        auto count(set_type const& active_pawns) const noexcept
        {
                using namespace xstd::support_literals;
                return active_pawns.none() ? 0_z : parallelize_lfold<left_up, right_up>(active_pawns);
        }

        template<template<int> class... Directions>
        auto parallelize_lfold(set_type const& active_pawns) const
        {
                return (parallelize<Directions<orientation>{}>(active_pawns) + ...);
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const noexcept
        {
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_pawns, state.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
