#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/raii.hpp>                 // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/actions/detail/builder.hpp>              // Builder
#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/detail/generate_king_jump.hpp>   // promote_en_passant
#include <dctl/core/actions/select/jump.hpp>                 // jumps
#include <dctl/core/board/angle.hpp>                         // rotate, inverse
#include <dctl/core/board/bearing.hpp>                       // bearing
#include <dctl/core/board/jump_sources.hpp>                     // jump_sources
#include <dctl/core/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/core/state/color_piece.hpp>                         // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>                         // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_same

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class Builder>
class generate<color_<Side>, pawns_, select::jump, Reverse, State, Builder>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = pawns_c;
        using  king_jumps = generate<to_move_, kings_, select::jump, Reverse, State, Builder>;
        using  board_type = board_t<Builder>;
        using  rules_type = rules_t<Builder>;
        using    set_type =   set_t<Builder>;

        template<int Direction>
        using pawn_jump_sources = jump_sources<board_type, Direction, short_ranged_tag>;

        constexpr static auto orientation = bearing_v<board_type, to_move_, Reverse>.value();

        template<class Iterator>
        constexpr static auto direction_v = rotate(ray::direction_v<Iterator>, inverse(angle{orientation}));

        Builder& m_builder;
public:
        explicit generate(Builder& b) noexcept
        :
                m_builder{b}
        {}

        auto operator()() const
        {
                if (m_builder.pieces(to_move_c, piece_c).empty()) {
                        return;
                }
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        directions();
                } else {
                        directions();
                }
        }
private:
        auto directions() const
        {
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right_up, left_up>();
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right_up, left_up, left_down, right_down>();
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right, right_up, up, left_up, left>();
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<right, right_up, up, left_up, left, left_down, down, right_down>();
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold() const
        {
                (... , sources<Directions<orientation>{}>());
        }

        template<int Direction>
        auto sources() const
        {
                pawn_jump_sources<Direction>{}(
                        m_builder.pieces(to_move_c, piece_c),
                        m_builder.targets(),
                        m_builder.pieces(empty_c)
                ).for_each([this](auto const from_sq) {
                        jump(along_ray<Direction>(from_sq));
                });
        }

        template<class Iterator>
        auto jump(Iterator const jumper) const
        {
                assert(is_onboard(jumper));
                raii::launch<Builder> guard{m_builder, *jumper};
                capture(std::next(jumper));
        }

        template<class Iterator>
        auto capture(Iterator const jumper) const
                -> void
        {
                assert(is_onboard(jumper));
                raii::capture<Builder> guard{m_builder, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                try_promotion(jumper);
        }

        template<class Iterator>
        auto try_promotion(Iterator jumper) const
        {
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (is_promotion(*jumper)) {
                                return on_promotion(jumper);
                        }
                        return try_next(jumper);
                } else {
                        if (next_target(jumper)) {
                                return;
                        }
                        if (is_promotion(*jumper)) {
                                return on_promotion(jumper);
                        }
                        return add_jump(*jumper);
                }
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::promotion<Builder> guard{m_builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        return on_king_jump(jumper);
                } else {
                        return add_jump(*jumper);
                }
        }

        template<class Iterator>
        auto on_king_jump(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        king_jumps_try_next(jumper);
                } else {
                        king_jumps_try_next(jumper);
                }
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                king_jumps{m_builder}.try_next_passing_promotion(jumper);
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!next_target(jumper)) {
                        add_jump(*jumper);
                }
        }

        template<class Iterator>
        auto next_target(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                        return scan(ray::mirror<up<orientation>{}>(jumper));
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_diagonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-90, +90>(jumper);
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> &&  is_orthogonal_jump_v<rules_type>) {
                        static_assert(!is_down(direction_v<Iterator>));

                        if constexpr (direction_v<Iterator> == right<orientation>{}) {
                                return turn_directions_lfold<right_up, up, left_up>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == right_up<orientation>{}) {
                                return turn_directions_lfold<right, up, left_up, left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == up<orientation>{}) {
                                return turn_directions_lfold<right, right_up, left_up, left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == left_up<orientation>{}) {
                                return turn_directions_lfold<right, right_up, up, left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == left<orientation>{}) {
                                return turn_directions_lfold<right_up, up, left_up>(jumper);
                        }
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> &&  is_orthogonal_jump_v<rules_type>) {
                        return rotate_directions_lfold<-135, -90, -45, +45, +90, +135>(jumper);
                }
        }

        template<template<int> class... Directions, class Iterator>
        auto turn_directions_lfold(Iterator jumper) const
        {
                return (... | scan(ray::turn<Directions<orientation>{}>(jumper)));
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (... | scan(ray::rotate<Directions>(jumper)));
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                return is_en_prise(std::next(jumper));
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && m_builder.is_target(jumper))) {
                        return false;
                }

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        auto add_jump(int const dest_sq) const
        {
                m_builder.finalize(dest_sq);
        }

        template<int Direction>
        auto along_ray(int const sq) const noexcept
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        auto is_promotion(int const sq) const // Throws: Nothing.
        {
                return board_type::promotion(Side).contains(sq);
        }
};

}       // namespace detail
}       // namespace dctl::core
