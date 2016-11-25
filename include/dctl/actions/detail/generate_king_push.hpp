#pragma once
#include <dctl/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/board/mask/push_targets.hpp>             // push_targets
#include <dctl/color_piece.hpp>                         // color, color_, king_
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t, value_t
#include <iterator>                                     // prev

namespace dctl {
namespace detail {

template<color Side, class Reverse, class State, class SequenceContainer>
class generate<color_<Side>, kings_, select::push, Reverse, State, SequenceContainer>
{
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = kings_c;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;

        template<int Direction>
        using push_targets = board::mask::push_targets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.value();
        SequenceContainer& m_actions;
public:
        explicit generate(SequenceContainer& a) noexcept
        :
                m_actions{a}
        {}

        auto operator()(State const& s) const
        {
                auto const sources = s.pieces(to_move_c, piece_c);
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        sources.for_each([&, this](auto const& from_sq){
                                ray_directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(from_sq, s.pieces(empty_c));
                        });
                } else {
                        if (sources.any()) {
                                wave_directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(sources, s.pieces(empty_c));
                        }
                }
        }
private:
        template<template<int> class... Directions>
        auto ray_directions_lfold(int const from, set_type const destinations) const
        {
                (... , serialize(along_ray<Directions<orientation>{}>(from), destinations));
        }

        template<template<int> class... Directions>
        auto wave_directions_lfold(set_type const sources, set_type const destinations) const
        {
                (... , serialize<Directions<orientation>{}>(sources, destinations));
        }

        template<class Iterator>
        auto serialize(Iterator const from, set_type const destinations) const
        {
                board::ray::classical(
                        from,
                        destinations
                ).for_each([this, from](auto const dest_sq){
                        m_actions.emplace_back(
                                *from,
                                dest_sq
                        );
                });
        }

        template<int Direction>
        auto serialize(set_type const sources, set_type const destinations) const
        {
                push_targets<Direction>{}(
                        sources,
                        destinations
                ).for_each([this](auto const dest_sq){
                        m_actions.emplace_back(
                                *std::prev(along_ray<Direction>(dest_sq)),
                                dest_sq
                        );
                });
        }

        template<int Direction>
        auto along_ray(int const sq) const
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace dctl
