#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // select
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/state/promotion.hpp>                     // is_promotion
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <xstd/type_traits.hpp>                         // value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class SequenceContainer>
class Generate<ToMove, Piece::pawn, select::push, Reverse, State, SequenceContainer>
{
        using action_type = xstd::value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = PushTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;
        SequenceContainer& actions;
public:
        Generate(SequenceContainer& a)
        :
                actions{a}
        {}

        auto operator()(State const& state) const
        {
                auto const active_pawns = pieces<ToMove, Piece::pawn>(state);
                if (active_pawns.any())
                        directions_lfold<left_up, right_up>(active_pawns, state.not_occupied());
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_pawns, set_type const not_occupied) const
        {
                return (... , targets<Directions<bearing.degrees()>{}>(active_pawns, not_occupied));
        }

        template<int Direction>
        auto targets(set_type const active_pawns, set_type const not_occupied) const
        {
                push_targets<Direction>{}(
                        active_pawns,
                        not_occupied
                ).for_each([this](auto const dest_sq){
                        actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
                                dest_sq,
                                is_promotion(dest_sq)
                        );
                });
        }

        template<int Direction>
        static auto along_ray(std::size_t const sq) noexcept
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t const sq) noexcept
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
