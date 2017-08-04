#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/builder.hpp> // builder
#include <dctl/core/actions/detail/raii.hpp>    // Launch, Capture, Visit, set_king_jump
#include <dctl/core/actions/select/jump.hpp>    // jump
#include <dctl/core/board/angle.hpp>            // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/jump_targets.hpp>     // jump_targets
#include <dctl/core/board/ray.hpp>              // make_iterator, rotate, mirror
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/meta.hpp>                   // map_reduce, comma, bit_or
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <cassert>                              // assert
#include <iterator>                             // prev
#include <type_traits>                          // is_same

namespace dctl::core {
namespace detail {

template<class...>
class king_jump;

template<color Side, class Reverse, class State>
class king_jump<color_<Side>, Reverse, State>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;

        template<class Iterator>
        constexpr static auto direction_v = rotate(ray::direction_v<Iterator>, inverse(angle{orientation}));

        constexpr static auto GCC7_ICE_WORKAROUND_is_long_ranged_king = is_long_ranged_king_v<rules_type>;
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                        return meta::map_reduce<king_jump_directions<rules_type, orientation>, meta::logical_or>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !jump_targets<board_type, Direction, king_range_category_t<rules_type>>{}(kings, s.targets(color_c<Side>, kings_c), s.pieces(empty_c)).empty();
                        });
                }
                return false;
        }

        template<class Builder>
        static auto generate(Builder& m_builder)
        {
                raii::set_king_jump<Builder> g1{m_builder};
                m_builder.pieces(color_c<Side>, kings_c).consume([&](auto from_sq) {
                        raii::launch<Builder> g2{m_builder, from_sq};
                        meta::map_reduce<king_jump_directions<rules_type, orientation>, meta::comma>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                if constexpr (GCC7_ICE_WORKAROUND_is_long_ranged_king) {
                                        auto const jumper = ray::make_iterator<board_type, Direction>(from_sq);
                                        if (auto const blocker = ray::king_jump_target<rules_type>(jumper, m_builder.pieces(occup_c)); !blocker.empty()) {
                                                if (auto const first = find_first<Direction>(blocker); m_builder.template targets<Direction>().contains(first)) {
                                                        capture(ray::make_iterator<board_type, Direction>(first), m_builder);
                                                }
                                        }
                                } else {
                                        auto const jumper = std::next(ray::make_iterator<board_type, Direction>(from_sq));
                                        if (is_onboard(jumper) && m_builder.is_target(jumper)) {
                                                capture(jumper, m_builder);
                                        }
                                }
                        });
                });
        }

        template<class Iterator, class Builder>
        static auto try_next_passing_promotion(Iterator jumper, Builder& m_builder)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(m_builder.with() == piece::pawns);
                assert(m_builder.into() == piece::kings);
                land(jumper, m_builder);
        }
private:
        template<class Iterator, class Builder>
        static auto capture(Iterator jumper, Builder& m_builder)
                -> void
        {
                assert(is_onboard(jumper));
                raii::capture<Builder> guard{m_builder, *jumper};
                land(std::next(jumper), m_builder);
        }

        template<class Iterator, class Builder>
        static auto land(Iterator jumper, Builder& m_builder)
        {
                assert(is_onboard(jumper));
                if (!next_target(jumper, m_builder)) {
                        halt(jumper, m_builder);
                }
        }

        template<class Iterator, class Builder>
        static auto halt(Iterator dest_sq, Builder& m_builder)
        {
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                        if (m_builder.is_last_jumped_king(*std::prev(dest_sq))) {
                                return m_builder.finalize(*dest_sq);
                        } else {
                                return add_sliding_jumps(dest_sq, m_builder);
                        }
                }
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                        return add_sliding_jumps(dest_sq, m_builder);
                }
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return m_builder.finalize(*dest_sq);
                }
        }

        template<class Iterator, class Builder>
        static auto add_sliding_jumps(Iterator dest_sq, Builder& m_builder)
        {
                // builder.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && m_builder.not_occupied(*dest_sq));
                do {
                        m_builder.finalize(*dest_sq++);
                } while (is_onboard(dest_sq) && m_builder.not_occupied(*dest_sq));
        }

        template<class Iterator, class Builder>
        static auto next_target(Iterator jumper, Builder& m_builder)
        {
                //raii::Visit<Builder> guard{m_builder, *jumper};
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan_turn(jumper, m_builder) | reverse(jumper, m_builder);
                } else {
                        return scan_turn(jumper, m_builder);
                }
        }

        template<class Iterator, class Builder>
        static auto reverse(Iterator jumper, Builder& m_builder)
        {
                static_assert(is_reverse_king_jump_v<rules_type>);
                return scan(ray::rotate<180>(jumper), m_builder);
        }

        template<class Iterator, class Builder>
        static auto scan_turn(Iterator jumper, Builder& m_builder)
        {
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan(jumper, m_builder) | turn(jumper, m_builder);
                } else {
                        constexpr auto Direction = ray::direction_v<Iterator>.value();

                        auto found_next = false;
                        auto ahead = ray::king_move_scan<rules_type, board_type>{}(*jumper, ray::move_index(Direction));
                        auto n = ahead.count();
                        if (ahead &= m_builder.pieces(occup_c); !ahead.empty()) {
                                auto const first = find_first<Direction>(ahead);
                                if (m_builder.template targets<Direction>().contains(first)) {
                                        capture(ray::make_iterator<board_type, Direction>(first), m_builder);
                                        found_next |= true;
                                }
                                n -= ray::blocker_and_beyond<rules_type, board_type>{}(first, ray::jump_index<rules_type>(Direction)).count();
                        }
                        do { found_next |= turn(jumper++, m_builder); } while(n--);
                        return found_next;
                }
        }

        template<class Iterator, class Builder>
        static auto turn(Iterator jumper, Builder& m_builder)
        {
                return meta::map_reduce<jump_rotations<rules_type>, meta::bit_or>{}([&](auto direction) {
                        return scan(ray::rotate<decltype(direction){}>(jumper), m_builder);
                });
        }

        template<class Iterator, class Builder>
        static auto scan(Iterator jumper, Builder& m_builder)
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        constexpr auto Direction = ray::direction_v<Iterator>.value();
                        if (auto const blocker = ray::king_jump_target<rules_type>(jumper, m_builder.pieces(occup_c)); !blocker.empty()) {
                                if (auto const first = find_first<Direction>(blocker); m_builder.template targets<Direction>().contains(first)) {
                                        capture(ray::make_iterator<board_type, Direction>(first), m_builder);
                                        return true;
                                }
                        }
                        return false;
                } else {
                        ++jumper;
                        if (is_onboard(jumper) && m_builder.is_target(jumper)) {
                                assert(is_onboard(std::next(jumper)));
                                capture(jumper, m_builder);
                                return true;
                        }
                        return false;
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
