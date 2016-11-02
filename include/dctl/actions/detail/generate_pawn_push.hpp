#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // select
#include <dctl/board/angle.hpp>                         // left_up, right_up
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/promotion.hpp>                // is_promotion
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/color_piece.hpp>                         // Color, color_constant, pawn_
#include <dctl/utility/type_traits.hpp>                 // board_t, set_t, value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State, class SequenceContainer>
class Generate<color_constant<Side>, pawn_, select::push, Reverse, State, SequenceContainer>
{
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = pawn_c;
        using  board_type = board_t<State>;
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
                if (auto const sources = state.pieces(to_move_c, piece_c); sources.any()) {
                        directions_lfold<board::right_up, board::left_up>(sources, state.pieces(none_c));
                }
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const destinations) const
        {
                (... , targets<Directions<orientation>{}>(sources, destinations));
        }

        template<int Direction>
        auto targets(set_type const sources, set_type const destinations) const
        {
                push_targets<Direction>{}(
                        sources,
                        destinations
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
                return board::mask::promotion_v<board_type, to_move_>.test(sq);
        }
};

}       // namespace detail
}       // namespace dctl
