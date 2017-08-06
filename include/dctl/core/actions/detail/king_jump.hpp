#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/builder.hpp> // builder
#include <dctl/core/actions/detail/pattern.hpp> // jump_targets
#include <dctl/core/actions/detail/raii.hpp>    // Launch, Capture, Visit, set_king_jump
#include <dctl/core/actions/select/jump.hpp>    // jump
#include <dctl/core/board/angle.hpp>            // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/ray.hpp>              // make_iterator, rotate, mirror
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_comma, foldl_bit_or
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

        template<class Direction>
        using rot = meta::integral_c<int, rotate(angle{Direction::value}, angle{orientation}).value()>;

        using king_jump_directions = meta::transform<rot, basic_king_jump_directions<rules_type>>;

        template<class Arg, class Direction>
        using is_forward = std::bool_constant<Arg::value == Direction::value>;

        template<class Arg, class Direction>
        using is_reverse = std::bool_constant<Arg::value == rotate_v<Direction::value, 180>>;

        template<class Arg, class Direction>
        using is_forward_or_reverse = std::disjunction<is_forward<Arg, Direction>, is_reverse<Arg, Direction>>;

        template<class Direction>
        using king_turn_directions = meta::remove_if_q<king_jump_directions, meta::bind_back<is_forward_or_reverse, Direction>>;

        constexpr static auto GCC7_ICE_WORKAROUND_is_long_ranged_king = is_long_ranged_king_v<rules_type>;
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const kings = s.pieces(color_c<Side>, kings_c); !kings.empty()) {
                        auto const enemy = s.targets(color_c<Side>, kings_c);
                        auto const empty = s.pieces(empty_c);
                        return meta::foldl_logical_or<king_jump_directions>{}([&](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                return !jump_targets<board_type, direction_v, king_range_category_t<rules_type>>{}(kings, enemy, empty)
                                        .empty()
                                ;
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
                        meta::foldl_comma<king_jump_directions>{}([&](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                if constexpr (GCC7_ICE_WORKAROUND_is_long_ranged_king) {
                                        if (auto const blocker = ray::king_jump_target<rules_type, board_type, direction_v>(from_sq, m_builder.pieces(occup_c)); !blocker.empty()) {
                                                if (auto const first = find_first<direction_v>(blocker); m_builder.template targets<direction_v>().contains(first)) {
                                                        capture(ray::make_iterator<board_type, direction_v>(first), m_builder);
                                                }
                                        }
                                } else {
                                        auto const jumper = std::next(ray::make_iterator<board_type, direction_v>(from_sq));
                                        if (board_type::is_onboard(*jumper) && m_builder.template is_target<direction_v>(*jumper)) {
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
                if (!next_target(jumper, m_builder)) {
                        halt(jumper, m_builder);
                }
        }
private:
        template<class Iterator, class Builder>
        static auto capture(Iterator jumper, Builder& m_builder)
                -> void
        {
                assert(board_type::is_onboard(*jumper));
                raii::capture<Builder> guard{m_builder, *jumper};
                ++jumper;
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
                assert(board_type::is_onboard(*dest_sq) && m_builder.not_occupied(*dest_sq));
                do {
                        m_builder.finalize(*dest_sq++);
                } while (board_type::is_onboard(*dest_sq) && m_builder.not_occupied(*dest_sq));
        }

        template<class Iterator, class Builder>
        static auto next_target(Iterator jumper, Builder& m_builder)
        {
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
                constexpr auto Direction = ray::direction_v<Iterator>.value();
                return meta::foldl_bit_or<king_turn_directions<meta::integral_c<int, Direction>>>{}([&](auto direction) {
                        return scan(ray::turn<decltype(direction){}>(jumper), m_builder);
                });
        }

        template<class Iterator, class Builder>
        static auto scan(Iterator jumper, Builder& m_builder)
        {
                constexpr auto Direction = ray::direction_v<Iterator>.value();
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        if (auto const blocker = ray::king_jump_target<rules_type, board_type, Direction>(*jumper, m_builder.pieces(occup_c)); !blocker.empty()) {
                                if (auto const first = find_first<Direction>(blocker); m_builder.template targets<Direction>().contains(first)) {
                                        capture(ray::make_iterator<board_type, Direction>(first), m_builder);
                                        return true;
                                }
                        }
                        return false;
                } else {
                        ++jumper;
                        if (board_type::is_onboard(*jumper) && m_builder.template is_target<Direction>(*jumper)) {
                                assert(board_type::is_onboard(*std::next(jumper)));
                                capture(jumper, m_builder);
                                return true;
                        }
                        return false;
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
