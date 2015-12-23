#pragma once
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <dctl/board/wave/patterns.hpp>                 // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::king, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        State const& state;

public:
        explicit Detect(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return detect(state.pieces(ToMove, Piece::king));
        }

        auto operator()(set_type const& active_kings) const noexcept
        {
                return detect(active_kings);
        }

private:
        auto detect(set_type const& active_kings) const noexcept
        {
                if (active_kings.none())
                        return false;

                return parallelize_lfold<left_up, right_up, left_down, right_down>(active_kings);
        }

        template<template<int> class... Directions>
        auto parallelize_lfold(set_type const& active_kings) const
        {
                return (parallelize<Directions<orientation>{}>(active_kings) || ...);
        }

        template<int Direction>
        auto parallelize(set_type const& active_kings) const noexcept
        {
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_kings, state.not_occupied()
                ).any();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
