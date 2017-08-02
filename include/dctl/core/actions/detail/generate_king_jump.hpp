#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/raii.hpp>                    // Launch, Capture, Visit, set_king_jump
#include <dctl/core/actions/detail/builder.hpp>                 // Builder
#include <dctl/core/actions/detail/generate_primary_fwd.hpp>    // generate (primary template)
#include <dctl/core/actions/select/jump.hpp>                    // jump
#include <dctl/core/board/angle.hpp>                            // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/ray.hpp>                              // make_iterator, rotate, mirror
#include <dctl/core/state/color_piece.hpp>                      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>                      // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/meta.hpp>                                   // map_reduce, comma, bit_or, tuple_c, int_c
#include <dctl/util/type_traits.hpp>                            // action_t, board_t, rules_t, set_t
#include <cassert>                                              // assert
#include <iterator>                                             // prev
#include <type_traits>                                          // is_same

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class Builder>
class generate<color_<Side>, kings_, select::jump, Reverse, State, Builder>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
        using king_jump_directions = std::conditional_t<
                is_orthogonal_jump_v<rules_type>,
                std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left_up<orientation>, left<orientation>, left_down<orientation>, down<orientation>, right_down<orientation>>,
                std::tuple<right_up<orientation>, left_up<orientation>, left_down<orientation>, right_down<orientation>>
        >;

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
                raii::set_king_jump<Builder> g1{m_builder};
                m_builder.pieces(color_c<Side>, kings_c).consume([this](auto const from_sq) {
                        raii::launch<Builder> g2{m_builder, from_sq};
                        meta::map_reduce<king_jump_directions, meta::comma>{}([from_sq, this](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const jumper = along_ray<Direction>(from_sq);
                                        if (auto const blocker = ray::king_jump_target<rules_type>(jumper, m_builder.pieces(occup_c)); !blocker.empty()) {
                                                if (auto const first = find_first<Direction>(blocker); m_builder.template targets<Direction>().contains(first)) {
                                                        capture(along_ray<Direction>(first));
                                                }
                                        }
                                } else {
                                        auto const jumper = std::next(along_ray<Direction>(from_sq));
                                        if (is_onboard(jumper) && m_builder.is_target(jumper)) {
                                                capture(jumper);
                                        }
                                }
                        });
                });
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
                        constexpr auto direction = ray::direction_v<Iterator>.value();
                        using Direction = meta::int_c<direction>;
                        constexpr auto index = Direction{} / 45;

                        auto found_next = false;
                        auto ahead = ray::king_move_targets<rules_type, board_type>{}(*jumper, index);
                        auto n = ahead.count();
                        if (ahead &= m_builder.pieces(occup_c); !ahead.empty()) {
                                auto const first = find_first<Direction{}>(ahead);
                                if (m_builder.template targets<direction>().contains(first)) {
                                        capture(along_ray<direction>(first));
                                        found_next |= true;
                                }
                                n -= ray::blocker_and_beyond<board_type>{}(first, index).count();
                        }
                        do { found_next |= turn(jumper++); } while(n--);
                        return found_next;
                }
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                using rotation_angles = std::conditional_t<
                        is_orthogonal_jump_v<rules_type>,
                        meta::tuple_c<-135, -90, -45, +45, +90, +135>,
                        meta::tuple_c<-90, +90>
                >;
                return meta::map_reduce<rotation_angles, meta::bit_or>{}([jumper, this](auto direction) {
                        return scan(ray::rotate<decltype(direction){}>(jumper));
                });
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        constexpr auto direction = ray::direction_v<Iterator>.value();
                        if (auto const blocker = ray::king_jump_target<rules_type>(jumper, m_builder.pieces(occup_c)); !blocker.empty()) {
                                if (auto const first = find_first<direction>(blocker); m_builder.template targets<direction>().contains(first)) {
                                        capture(along_ray<direction>(first));
                                        return true;
                                }
                        }
                        return false;
                } else {
                        return is_en_prise(std::next(jumper));
                }
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (is_onboard(jumper) && m_builder.is_target(jumper)) {
                        assert(is_onboard(std::next(jumper)));
                        capture(jumper);
                        return true;
                }
                return false;
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
