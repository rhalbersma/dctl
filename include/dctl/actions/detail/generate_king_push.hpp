#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/color_piece.hpp>                         // Color, color_constant, king_
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t, value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State, class SequenceContainer>
class Generate<color_constant<Side>, king_, select::push, Reverse, State, SequenceContainer>
{
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = king_c;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = board::mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.degrees();
        SequenceContainer& actions;
public:
        explicit Generate(SequenceContainer& a) noexcept
        :
                actions{a}
        {}

        auto operator()(State const& state) const
        {
                auto const sources = state.pieces(to_move_c, piece_c);
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        sources.for_each([&, this](auto const& from_sq){
                                ray_directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(from_sq, state.pieces(none_c));
                        });
                } else {
                        if (sources.any()) {
                                wave_directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(sources, state.pieces(none_c));
                        }
                }
        }
private:
        template<template<int> class... Directions>
        auto ray_directions_lfold(std::size_t const from, set_type const destinations) const
        {
                (... , ray_targets(along_ray<Directions<orientation>{}>(from), destinations));
        }

        template<template<int> class... Directions>
        auto wave_directions_lfold(set_type const sources, set_type const destinations) const
        {
                (... , wave_targets<Directions<orientation>{}>(sources, destinations));
        }

        template<class Iterator>
        auto ray_targets(Iterator const from, set_type const destinations) const
        {
                board::ray::classical(
                        from,
                        destinations
                ).for_each([this, from](auto const dest_sq){
                        actions.emplace_back(
                                *from,
                                dest_sq
                        );
                });
        }

        template<int Direction>
        auto wave_targets(set_type const sources, set_type const destinations) const
        {
                push_targets<Direction>{}(
                        sources,
                        destinations
                ).for_each([this](auto const dest_sq){
                        actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
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
