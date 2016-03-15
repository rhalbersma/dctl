#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t, value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class Sequence>
class Generate<ToMove, Piece::king, select::push, Reverse, State, Sequence>
{
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using wave_push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
        State const& state;
        Sequence& actions;
public:
        Generate(State const& s, Sequence& a) noexcept
        :
                state{s},
                actions{a}
        {}

        auto operator()() const
        {
                king_range_dispatch(king_range_category_t<rules_type>{});
        }
private:
        auto king_range_dispatch(short_ranged_tag) const
        {
                auto const active_kings = pieces<ToMove, Piece::king>(state);
                if (active_kings.any())
                        wave_directions_lfold<left_up, right_up, left_down, right_down>(active_kings, state.not_occupied());
        }

        auto king_range_dispatch(long_ranged_tag) const
        {
                pieces<ToMove, Piece::king>(state).for_each([&](auto const& from_sq){
                        ray_directions_lfold<left_up, right_up, left_down, right_down>(from_sq, state.not_occupied());
                });
        }

        template<template<int> class... Directions>
        auto wave_directions_lfold(set_type const active_kings, set_type const not_occupied) const
        {
                return (..., wave_targets<Directions<bearing.degrees()>{}>(active_kings, not_occupied));
        }

        template<template<int> class... Directions>
        auto ray_directions_lfold(std::size_t const from, set_type const not_occupied) const
        {
                return (..., ray_targets(along_ray<Directions<bearing.degrees()>{}>(from), not_occupied));
        }

        template<int Direction>
        auto wave_targets(set_type const active_kings, set_type const not_occupied) const
        {
                wave_push_targets<Direction>{}(
                        active_kings,
                        not_occupied
                ).for_each([this](auto const& dest_sq){
                        actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
                                dest_sq,
                                state
                        );
                });
        }

        template<class Iterator>
        auto ray_targets(Iterator const from, set_type const not_occupied) const
        {
                ray::classical(
                        from,
                        not_occupied
                ).for_each([this, from](auto const& dest_sq){
                        actions.emplace_back(
                                *from,
                                dest_sq,
                                state
                        );
                });
        }

        template<int Direction>
        static auto along_ray(std::size_t const sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
