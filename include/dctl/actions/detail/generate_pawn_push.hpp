#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // select
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/mask/promotion.hpp>                      // is_promotion
#include <dctl/mask/push_targets.hpp>                   // push_targets
#include <dctl/piece.hpp>                               // pawn_type
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t
#include <xstd/type_traits.hpp>                         // value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<class Color, class Reverse, class State, class SequenceContainer>
class Generate<Color, pawn_type, select::push, Reverse, State, SequenceContainer>
{
        using action_type = xstd::value_t<SequenceContainer>;
        using  board_type = board_t<State>;
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
                auto const active_pawns = state.pieces(Color{}, pawn_type{});
                if (active_pawns.any())
                        directions_lfold<left_up, right_up>(active_pawns, state.not_occupied());
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const active_pawns, set_type const not_occupied) const
        {
                (... , targets<Directions<bearing.degrees>{}>(active_pawns, not_occupied));
        }

        template<int Direction>
        auto targets(set_type const active_pawns, set_type const not_occupied) const
        {
                push_targets<Direction>{}(
                        active_pawns,
                        not_occupied
                ).for_each([this](auto const dest_sq){
                        actions.emplace_back(
                                *std::prev(this->along_ray<Direction>(dest_sq)),
                                dest_sq,
                                this->is_promotion(dest_sq)
                        );
                });
        }

        template<int Direction>
        auto along_ray(std::size_t const sq) const noexcept
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }

        auto is_promotion(std::size_t const sq) const // Throws: Nothing.
        {
                return mask::promotion_v<board_type, Color>.test(sq);
        }
};

}       // namespace detail
}       // namespace dctl
