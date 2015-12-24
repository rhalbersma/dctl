#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // select
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/state/promotion.hpp>                     // is_promotion
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t, value_t
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class Sequence>
class Generate<ToMove, Piece::pawn, select::push, Reverse, State, Sequence>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        State const& state;
        Sequence& actions;

public:
        Generate(State const& s, Sequence& a)
        :
                state{s},
                actions{a}
        {}

        auto operator()() const
        {
                sources(state.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const
        {
                sources(active_pawns);
        }

private:
        auto sources(set_type const& active_pawns) const
        {
                if (active_pawns.any())
                        directions_lfold<left_up, right_up>(active_pawns);
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const& active_pawns) const
        {
                return (targets<Directions<orientation>{}>(active_pawns) , ...);
        }

        template<int Direction>
        auto targets(set_type const& active_pawns) const
        {
                push_targets<Direction>{}(
                        active_pawns, state.not_occupied()
                ).for_each([&](auto const& dest_sq){
                        actions.emplace_back(*std::prev(along_ray<Direction>(dest_sq)), dest_sq, is_promotion(dest_sq), ToMove);
                });
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
