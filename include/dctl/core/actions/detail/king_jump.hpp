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

        template<class Arg>
        using oriented = meta::integral_c<int, rotate_v<Arg::value, orientation>>;

        using king_jump_directions = meta::transform<oriented, basic_king_jump_directions<rules_type>>;

        template<class Arg, class Direction>
        using is_forward = std::bool_constant<Arg::value == Direction::value>;

        template<class Arg, class Direction>
        using is_reverse = std::bool_constant<Arg::value == rotate_v<Direction::value, 180>>;

        template<class Arg, class Direction>
        using is_forward_or_reverse = std::disjunction<is_forward<Arg, Direction>, is_reverse<Arg, Direction>>;

        template<int Direction>
        using king_scan_directions = meta::remove_if_q<king_jump_directions, meta::bind_back<is_reverse, meta::integral_c<int, Direction>>>;

        template<int Direction>
        using king_turn_directions = meta::remove_if_q<king_jump_directions, meta::bind_back<is_forward_or_reverse, meta::integral_c<int, Direction>>>;

        constexpr static auto GCC7_ICE_WORKAROUND_is_long_ranged_king = is_long_ranged_king_v<rules_type>;
public:
        static auto detect(State const& s) noexcept
        {
                auto const kings = s.pieces(color_c<Side>, kings_c);
                if (kings.empty()) { return false; }
                return meta::foldl_logical_or<king_jump_directions>{}([&, targets = s.targets(color_c<Side>, kings_c), empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return !jump_targets<board_type, direction_v, king_range_category_t<rules_type>>{}(kings, targets, empty).empty();
                });
        }

        template<class Builder>
        static auto generate(Builder& builder)
        {
                raii::set_king_jump<Builder> g1{builder};
                builder.pieces(color_c<Side>, kings_c).consume([&](auto from_sq) {
                        raii::launch<Builder> g2{builder, from_sq};
                        meta::foldl_comma<king_jump_directions>{}([&, occup = builder.pieces(occup_c), targets = builder.targets(), empty = builder.pieces(empty_c)](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                if constexpr (GCC7_ICE_WORKAROUND_is_long_ranged_king) {
                                        auto const blocker = king_jump_scan<rules_type, board_type, direction_v>(from_sq) & occup;
                                        if (blocker.empty()) { return; }
                                        auto const first = find_first<direction_v>(blocker);
                                        if (!jump_targets<board_type, direction_v>{}(targets, empty).contains(first)) { return; }
                                        capture<direction_v>(first, builder);
                                } else {
                                        if (jump_targets<board_type, direction_v>{}(set_type{from_sq}, targets, empty).empty()) { return; }
                                        capture<direction_v>(next<board_type, direction_v>{}(from_sq), builder);
                                }
                        });
                });
        }

        template<int Direction, class Builder>
        static auto next_target_passing_promotion(int sq, Builder& builder)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(builder.with() == piece::pawns);
                builder.into(piece::kings);
                auto const n [[maybe_unused]] = king_moves<rules_type, board_type, Direction>{}(sq, builder.pieces(occup_c)).count();
                return next_target<Direction>(sq, n, builder);
        }
private:
        template<int Direction, class Builder>
        static auto capture(int sq, Builder& builder)
                -> void
        {
                raii::capture<Builder> guard{builder, sq};
                auto const n [[maybe_unused]] = king_moves<rules_type, board_type, Direction>{}(sq, builder.pieces(occup_c)).count();
                advance<board_type, Direction>{}(sq);
                if (!next_target<Direction>(sq, n, builder)) {
                        if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                                if (builder.is_last_jumped_king(prev<board_type, Direction>{}(sq))) {
                                        return builder.finalize(sq);
                                } else {
                                        return add_sliding_jumps<Direction>(sq, n, builder);
                                }
                        }
                        if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                                return add_sliding_jumps<Direction>(sq, n, builder);
                        }
                        if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                                return builder.finalize(sq);
                        }
                }
        }

        template<int Direction, class Builder>
        static auto add_sliding_jumps(int sq, int n, Builder& builder)
        {
                do {
                        builder.finalize(sq);
                        advance<board_type, Direction>{}(sq);
                } while (--n);
        }

        template<int Direction, class Builder>
        static auto next_target(int sq, int n [[maybe_unused]], Builder& builder)
        {
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan<king_jump_directions>(sq, builder);
                } else if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan<king_scan_directions<Direction>>(sq, builder);
                } else {
                        auto found_next = scan<king_scan_directions<Direction>>(sq, builder);
                        while (--n) {
                                advance<board_type, Direction>{}(sq);
                                found_next |= scan<king_turn_directions<Direction>>(sq, builder);
                        }
                        return found_next;
                }
        }

        template<class Directions, class Builder>
        static auto scan(int sq, Builder& builder)
        {
                return meta::foldl_bit_or<Directions>{}([&, targets = builder.targets(), empty = builder.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        if constexpr (is_long_ranged_king_v<rules_type>) {
                                auto const blocker = king_jump_scan<rules_type, board_type, direction_v>(sq) & builder.pieces(occup_c);
                                if (blocker.empty()) { return false; }
                                auto const first = find_first<direction_v>(blocker);
                                if (!jump_targets<board_type, direction_v>{}(targets, empty).contains(first)) { return false; }
                                capture<direction_v>(first, builder);
                        } else {
                                if (jump_targets<board_type, direction_v>{}(set_type{sq}, targets, empty).empty()) { return false; }
                                capture<direction_v>(next<board_type, direction_v>{}(sq), builder);
                        }
                        return true;
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
