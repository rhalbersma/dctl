#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/patterns.hpp>                      // push_targets
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                         // value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class SequenceContainer>
class Generate<ToMove, Piece::king, select::push, Reverse, State, SequenceContainer>
{
        using action_type = xstd::value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = board::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
        SequenceContainer& actions;
public:
        Generate(SequenceContainer& a)
        :
                actions{a}
        {}

        auto operator()(State const& state) const
        {
                king_range_dispatch(state, king_range_category_t<rules_type>{});
        }
private:
        auto king_range_dispatch(State const& state, short_ranged_tag) const
        {
                auto const active_kings = pieces<ToMove, Piece::king>(state);
                if (active_kings.any())
                        wave_directions_lfold<left_up, right_up, left_down, right_down>(active_kings, state.not_occupied());
        }

        auto king_range_dispatch(State const& state, long_ranged_tag) const
        {
                pieces<ToMove, Piece::king>(state).for_each([&, this](auto const& from_sq){
                        this->ray_directions_lfold<left_up, right_up, left_down, right_down>(from_sq, state.not_occupied());
                });
        }

        template<template<int> class... Directions>
        auto wave_directions_lfold(set_type const active_kings, set_type const not_occupied) const
        {
                (... , wave_targets<Directions<bearing.degrees()>{}>(active_kings, not_occupied));
        }

        template<template<int> class... Directions>
        auto ray_directions_lfold(std::size_t const from, set_type const not_occupied) const
        {
                (... , ray_targets(along_ray<Directions<bearing.degrees()>{}>(from), not_occupied));
        }

        template<int Direction>
        auto wave_targets(set_type const active_kings, set_type const not_occupied) const
        {
                push_targets<Direction>{}(
                        active_kings,
                        not_occupied
                ).for_each([this](auto const dest_sq){
                        actions.emplace_back(
                                *std::prev(this->along_ray<Direction>(dest_sq)),
                                dest_sq
                        );
                });
        }

        template<class Iterator>
        auto ray_targets(Iterator const from, set_type const not_occupied) const
        {
                board::ray::classical(
                        from,
                        not_occupied
                ).for_each([this, from](auto const dest_sq){
                        actions.emplace_back(
                                *from,
                                dest_sq
                        );
                });
        }

        template<int Direction>
        auto along_ray(std::size_t const sq) const
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
