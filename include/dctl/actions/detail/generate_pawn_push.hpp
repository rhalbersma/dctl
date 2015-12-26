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
#include <cstddef>                                      // size_t
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
        set_type const active_pawns;
        set_type const not_occupied;
        Sequence& actions;
public:
        Generate(State const& state, Sequence& a) noexcept
        :
                active_pawns{state.pieces(ToMove, Piece::pawn)},
                not_occupied{state.not_occupied()},
                actions{a}
        {}

        auto operator()() const
        {
                if (active_pawns.any())
                        directions_lfold<left_up, right_up>();
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold() const
        {
                return (targets<Directions<orientation>{}>() , ...);
        }

        template<int Direction>
        auto targets() const
        {
                push_targets<Direction>{}(active_pawns, not_occupied).for_each([this](auto const& dest_sq){
                        actions.emplace_back(*std::prev(along_ray<Direction>(dest_sq)), dest_sq, is_promotion(dest_sq), ToMove);
                });
        }

        template<int Direction>
        static auto along_ray(std::size_t sq) noexcept
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq) noexcept
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
