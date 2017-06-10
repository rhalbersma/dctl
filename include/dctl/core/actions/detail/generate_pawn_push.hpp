#pragma once
#include <dctl/core/actions/detail/generate_primary_fwd.hpp>    // generate (primary template)
#include <dctl/core/actions/select/push.hpp>                    // select
#include <dctl/core/board/angle.hpp>                            // left_up, right_up
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/push_targets.hpp>                // push_targets
#include <dctl/core/board/ray.hpp>                              // make_iterator
#include <dctl/core/state/color_piece.hpp>                      // color, color_, pawn_
#include <dctl/util/type_traits.hpp>                            // board_t, set_t, value_t
#include <iterator>                                             // prev

#include "../../board/traits.hpp"                           // promotion

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class SequenceContainer>
class generate<color_<Side>, pawns_, select::push, Reverse, State, SequenceContainer>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = pawns_c;
        using  board_type = board_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using pawn_push_targets = push_targets<board_type, Direction, short_ranged_tag>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>.value();
        SequenceContainer& m_actions;
public:
        explicit generate(SequenceContainer& seq) noexcept
        :
                m_actions{seq}
        {}

        auto operator()(State const& s) const
        {
                if (auto const sources = s.pieces(to_move_c, piece_c); !sources.empty()) {
                        directions_lfold<right_up, left_up>(sources, s.pieces(empty_c));
                }
        }
private:
        template<template<int> class... Directions>
        auto directions_lfold(set_type const sources, set_type const destinations) const
        {
                (... , serialize<Directions<orientation>{}>(sources, destinations));
        }

        template<int Direction>
        auto serialize(set_type const sources, set_type const destinations) const
        {
                pawn_push_targets<Direction>{}(
                        sources,
                        destinations
                ).for_each([this](auto const dest_sq){
                        m_actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
                                dest_sq,
                                is_promotion(dest_sq)
                        );
                });
        }

        template<int Direction>
        auto along_ray(int const sq) const noexcept
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        auto is_promotion(int const sq) const // Throws: Nothing.
        {
                return promotion_v<board_type, to_move_>.test(sq);
        }
};

}       // namespace detail
}       // namespace dctl::core
