#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/raii.hpp>                 // Launch, Capture, Visit, set_king_jump
#include <dctl/core/actions/detail/builder.hpp>              // Builder
#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/select/jump.hpp>                 // jump
#include <dctl/core/board/angle.hpp>                         // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/core/board/bearing.hpp>                       // bearing
#include <dctl/core/board/ray.hpp>                           // make_iterator, rotate, mirror
#include <dctl/core/state/color_piece.hpp>                         // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>                         // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                        // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_same

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class Builder>
class generate<color_<Side>, kings_, select::jump, Reverse, State, Builder>
{
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        constexpr static auto piece_c = kings_c;
        using  board_type = board_t<Builder>;
        using  rules_type = rules_t<Builder>;
        using    set_type =   set_t<Builder>;

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
                raii::set_king_jump<Builder> guard{m_builder};
                sources();
        }

        template<class Iterator>
        auto try_next_passing_promotion(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(m_builder.with() == piece::pawns);
                assert(m_builder.into() == piece::kings);
                try_next(jumper);
        }
private:
        auto sources() const
        {
                xstd::for_each(m_builder.pieces(to_move_c, piece_c), [this](auto const from_sq) {
                        raii::launch<Builder> guard{m_builder, from_sq};
                        if constexpr (is_orthogonal_jump_v<rules_type>) {
                                directions_lfold<right, right_up, up, left_up, left, left_down, down, right_down>(from_sq);
                        } else {
                                directions_lfold<right_up, left_up, left_down, right_down>(from_sq);
                        }
                });
        }

        template<template<int> class... Directions>
        auto directions_lfold(int from_sq) const
        {
                (... , first_target(along_ray<Directions<orientation>{}>(from_sq)));
        }

        template<class Iterator>
        auto first_target(Iterator jumper) const
        {
                slide(jumper, m_builder.template path<ray::direction_v<Iterator>.value()>());
                if (is_onboard(jumper) && m_builder.is_target(jumper)) {
                        assert(is_onboard(std::next(jumper)));
                        capture(jumper);
                }
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
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
                try_next(jumper);
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!next_target(jumper)) {
                        halt(jumper);
                }
        }

        template<class Iterator>
        auto next_target(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{m_builder, *jumper};
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan_turn(jumper) | reverse(jumper);
                } else {
                        return scan_turn(jumper);
                }
        }

        template<class Iterator>
        auto reverse(Iterator jumper) const
        {
                static_assert(is_reverse_king_jump_v<rules_type>);
                return scan(ray::rotate<180>(jumper));
        }

        template<class Iterator>
        auto scan_turn(Iterator jumper) const
        {
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan(jumper) | turn(jumper);
                } else {
                        // builder.template path<Direction>() would be an ERROR here
                        // because we need all landing squares rather than the directional launching squares subset
                        assert(is_onboard(jumper) && m_builder.not_occupied(*jumper));
                        auto found_next = turn(jumper);
                        auto slider = std::next(jumper);
                        while (is_onboard(slider) && m_builder.not_occupied(*slider)) {
                                found_next |= turn(slider);
                                ++slider;
                        }
                        return found_next |= is_en_prise(slider);
                }
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                if constexpr (is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-135, -90, -45, +45, +90, +135>(jumper);
                } else {
                        static_assert(is_diagonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-90, +90>(jumper);
                }
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (... | scan(ray::rotate<Directions>(jumper)));
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                slide(jumper, m_builder.template path<ray::direction_v<Iterator>.value()>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        auto slide(Iterator& jumper, set_type const path [[maybe_unused]]) const
        {
                assert(is_onboard(jumper));
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        do {
                                ++jumper;
                        } while (is_onboard(jumper) && path.contains(*jumper));
                } else {
                        ++jumper;
                }
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

        template<class Iterator>
        auto halt(Iterator dest_sq) const
        {
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                        if (m_builder.is_last_jumped_king(*std::prev(dest_sq))) {
                                return add_halting_jump(*dest_sq);
                        } else {
                                return add_sliding_jumps(dest_sq);
                        }
                }
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                        return add_sliding_jumps(dest_sq);
                }
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return add_halting_jump(*dest_sq);
                }
        }

        template<class Iterator>
        auto add_sliding_jumps(Iterator dest_sq) const
        {
                // builder.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && m_builder.not_occupied(*dest_sq));
                do {
                        add_halting_jump(*dest_sq++);
                } while (is_onboard(dest_sq) && m_builder.not_occupied(*dest_sq));
        }

        auto add_halting_jump(int const dest_sq) const
        {
                m_builder.finalize(dest_sq);
        }

        template<int Direction>
        auto along_ray(int const sq) const
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace dctl::core
