#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // select
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/promotion.hpp>                // is_promotion
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/color_piece.hpp>                         // Color, color_constant, pawn_type
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t, value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State, class SequenceContainer>
class Generate<color_constant<Side>, pawn_type, select::push, Reverse, State, SequenceContainer>
{
        using  color_type = color_constant<Side>;
        using  piece_type = pawn_type;
        using action_type = value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = board::mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, color_type, Reverse>.degrees();
        SequenceContainer& actions;
public:
        explicit Generate(SequenceContainer& a) noexcept
        :
                actions{a}
        {}

        auto operator()(State const& state) const
        {
                if (auto const generator = state.pieces(color_type{}, piece_type{}); generator.any()) {
                        directions_lfold<board::right_up, board::left_up>(generator, state.pieces(none_type{}));
                }
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const generator, set_type const propagator) const
        {
                (... , targets<Directions<orientation>{}>(generator, propagator));
        }

        template<int Direction>
        auto targets(set_type const generator, set_type const propagator) const
        {
                push_targets<Direction>{}(
                        generator,
                        propagator
                ).for_each([this](auto const dest_sq){
                        actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
                                dest_sq,
                                is_promotion(dest_sq)
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
                return board::mask::promotion_v<board_type, color_type>.test(sq);
        }
};

}       // namespace detail
}       // namespace dctl
