#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
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

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        set_type const active_kings;
        set_type const not_occupied;
        Sequence& actions;
public:
        Generate(set_type const& k, set_type const& e, Sequence& a) noexcept
        :
                active_kings{k},
                not_occupied{e},
                actions{a}
        {}

        Generate(State const& state, Sequence& a) noexcept
        :
                active_kings{state.pieces(ToMove, Piece::king)},
                not_occupied{state.not_occupied()},
                actions{a}
        {}

        auto operator()() const
        {
                king_range_dispatch(king_range_category_t<rules_type>{});
        }
private:
        auto king_range_dispatch(short_ranged_tag) const
        {
                if (active_kings.any())
                        wave_directions_lfold<left_up, right_up, left_down, right_down>();
        }

        auto king_range_dispatch(long_ranged_tag) const
        {
                active_kings.for_each([&](auto const& from_sq){
                        ray_directions_lfold<left_up, right_up, left_down, right_down>(from_sq);
                });
        }

        template<template<int> class... Directions>
        auto wave_directions_lfold() const
        {
                return (wave_targets<Directions<orientation>{}>() , ...);
        }

        template<template<int> class... Directions>
        auto ray_directions_lfold(std::size_t from) const
        {
                return (ray_targets(along_ray<Directions<orientation>{}>(from)) , ...);
        }

        template<int Direction>
        auto wave_targets() const
        {
                wave_push_targets<Direction>{}(active_kings, not_occupied).for_each([this](auto const& dest_sq){
                        actions.emplace_back(*std::prev(along_ray<Direction>(dest_sq)), dest_sq, ToMove);
                });
        }

        template<class Iterator>
        auto ray_targets(Iterator from) const
        {
                ray::classical(from, not_occupied).for_each([this, from](auto const& dest_sq){
                        actions.emplace_back(*from, dest_sq, ToMove);
                });
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
