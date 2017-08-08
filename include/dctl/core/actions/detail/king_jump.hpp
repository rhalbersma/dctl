#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/builder.hpp> // builder
#include <dctl/core/actions/detail/pattern.hpp> // jump_targets
#include <dctl/core/actions/detail/raii.hpp>    // Launch, Capture, Visit, set_king_jump
#include <dctl/core/actions/detail/tables.hpp>  // king_jump_scan, king_move_scan, blocker_and_beyond
#include <dctl/core/actions/select/jump.hpp>    // jump
#include <dctl/core/board/angle.hpp>            // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_comma, foldl_bit_or
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <cassert>                              // assert
#include <iterator>                             // prev
#include <type_traits>                          // bool_constant

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

        template<int Direction>
        using king_turn_directions = meta::remove_if_q<king_jump_directions, meta::bind_back<is_forward_or_reverse, meta::integral_c<int, Direction>>>;

        constexpr static auto GCC7_ICE_WORKAROUND_is_long_ranged_king = is_long_ranged_king_v<rules_type>;
public:
        static auto detect(State const& s) noexcept
        {
                auto const kings = s.pieces(color_c<Side>, kings_c);
                if (kings.empty()) {
                        return false;
                }
                return meta::foldl_logical_or<king_jump_directions>{}([&, targets = s.targets(color_c<Side>, kings_c), empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return !jump_targets<board_type, direction_v, king_range_category_t<rules_type>>{}(kings, targets, empty)
                                .empty()
                        ;
                });
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
                                        if (auto const blocker = king_jump_scan<rules_type, board_type, direction_v>(from_sq) & m_builder.pieces(occup_c); !blocker.empty()) {
                                                if (auto const first = find_first<direction_v>(blocker); m_builder.template targets<direction_v>().contains(first)) {
                                                        capture<direction_v>(first, m_builder);
                                                }
                                        }
                                } else {
                                        if (!(king_jump_scan<rules_type, board_type, direction_v>(from_sq) & m_builder.template targets<direction_v>()).empty()) {
                                                capture<direction_v>(next<board_type, direction_v>{}(from_sq), m_builder);
                                        }
                                }
                        });
                });
        }

        template<int Direction, class Builder>
        static auto try_next_passing_promotion(int jumper, Builder& m_builder)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(m_builder.with() == piece::pawns);
                assert(m_builder.into() == piece::kings);
                if (!next_target<Direction>(jumper, m_builder)) {
                        halt<Direction>(jumper, m_builder);
                }
        }
private:
        template<int Direction, class Builder>
        static auto capture(int jumper, Builder& m_builder)
                -> void
        {
                raii::capture<Builder> guard{m_builder, jumper};
                advance<board_type, Direction>{}(jumper);
                if (!next_target<Direction>(jumper, m_builder)) {
                        halt<Direction>(jumper, m_builder);
                }
        }

        template<int Direction, class Builder>
        static auto halt(int dest_sq, Builder& m_builder)
        {
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                        if (m_builder.is_last_jumped_king(prev<board_type, Direction>{}(dest_sq))) {
                                return m_builder.finalize(dest_sq);
                        } else {
                                return add_sliding_jumps<Direction>(dest_sq, m_builder);
                        }
                }
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                        return add_sliding_jumps<Direction>(dest_sq, m_builder);
                }
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return m_builder.finalize(dest_sq);
                }
        }

        template<int Direction, class Builder>
        static auto add_sliding_jumps(int dest_sq, Builder& m_builder)
        {
                do {
                        m_builder.finalize(dest_sq);
                        advance<board_type, Direction>{}(dest_sq);
                } while (board_type::is_onboard(dest_sq) && m_builder.pieces(empty_c).contains(dest_sq));
        }

        template<int Direction, class Builder>
        static auto next_target(int jumper, Builder& m_builder)
        {
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan_turn<Direction>(jumper, m_builder) | reverse<Direction>(jumper, m_builder);
                } else {
                        return scan_turn<Direction>(jumper, m_builder);
                }
        }

        template<int Direction, class Builder>
        static auto reverse(int jumper, Builder& m_builder)
        {
                static_assert(is_reverse_king_jump_v<rules_type>);
                return scan<rotate_v<Direction, 180>>(jumper, m_builder);
        }

        template<int Direction, class Builder>
        static auto scan_turn(int jumper, Builder& m_builder)
        {
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan<Direction>(jumper, m_builder) | turn<Direction>(jumper, m_builder);
                } else {
                        auto found_next = false;
                        auto ahead = king_move_scan<rules_type, board_type, Direction>(jumper);
                        auto n = ahead.count();
                        if (ahead &= m_builder.pieces(occup_c); !ahead.empty()) {
                                auto const first = find_first<Direction>(ahead);
                                if (m_builder.template targets<Direction>().contains(first)) {
                                        capture<Direction>(first, m_builder);
                                        found_next |= true;
                                }
                                n -= blocker_and_beyond<rules_type, board_type, Direction>(first).count();
                        }
                        do {
                                found_next |= turn<Direction>(jumper, m_builder);
                                advance<board_type, Direction>{}(jumper);
                        } while(n--);
                        return found_next;
                }
        }

        template<int Direction, class Builder>
        static auto turn(int jumper, Builder& m_builder)
        {
                return meta::foldl_bit_or<king_turn_directions<Direction>>{}([&](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return scan<direction_v>(jumper, m_builder);
                });
        }

        template<int Direction, class Builder>
        static auto scan(int jumper, Builder& m_builder)
        {
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        if (auto const blocker = king_jump_scan<rules_type, board_type, Direction>(jumper) & m_builder.pieces(occup_c); !blocker.empty()) {
                                if (auto const first = find_first<Direction>(blocker); m_builder.template targets<Direction>().contains(first)) {
                                        capture<Direction>(first, m_builder);
                                        return true;
                                }
                        }
                        return false;
                } else {
                        if (!(king_jump_scan<rules_type, board_type, Direction>(jumper) & m_builder.template targets<Direction>()).empty()) {
                                capture<Direction>(next<board_type, Direction>{}(jumper), m_builder);
                                return true;
                        }
                        return false;
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
