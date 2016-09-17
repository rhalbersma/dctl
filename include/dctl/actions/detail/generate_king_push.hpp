#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/mask/push_targets.hpp>                   // push_targets
#include <dctl/piece.hpp>                               // king_type
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                         // value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<class Color, class Reverse, class State, class SequenceContainer>
class Generate<Color, king_type, select::push, Reverse, State, SequenceContainer>
{
        using action_type = xstd::value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, Color, Reverse::value>;
        SequenceContainer& actions;
public:
        explicit Generate(SequenceContainer& a) noexcept
        :
                actions{a}
        {}

        auto operator()(State const& state) const
        {
                if constexpr (std::is_same<king_range_category_t<rules_type>, short_ranged_tag>{}) {
                        if (auto const active_kings = state.pieces(Color{}, king_type{}); active_kings.any())
                                wave_directions_lfold<left_up, right_up, left_down, right_down>(active_kings, state.not_occupied());
                }

                if constexpr (std::is_same<king_range_category_t<rules_type>,  long_ranged_tag>{}) {
                        state.pieces(Color{}, king_type{}).for_each([&, this](auto const& from_sq){
                                this->ray_directions_lfold<left_up, right_up, left_down, right_down>(from_sq, state.not_occupied());
                        });
                }
        }
private:
        template<template<int> class... Directions>
        auto wave_directions_lfold(set_type const active_kings, set_type const not_occupied) const
        {
                (... , wave_targets<Directions<bearing.degrees>{}>(active_kings, not_occupied));
        }

        template<template<int> class... Directions>
        auto ray_directions_lfold(std::size_t const from, set_type const not_occupied) const
        {
                (... , ray_targets(along_ray<Directions<bearing.degrees>{}>(from), not_occupied));
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
}       // namespace dctl
