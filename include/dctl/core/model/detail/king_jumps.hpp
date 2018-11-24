#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/model/detail/builder.hpp>   // builder
#include <dctl/core/model/detail/pattern.hpp>   // jump_targets
#include <dctl/core/model/detail/raii.hpp>      // capture, lift, set_king_jump
#include <dctl/core/model/detail/tables.hpp>    // king_jumps, king_moves
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/hana/or.hpp>                    // or_
#include <boost/hana/remove_if.hpp>             // remove_if
#include <algorithm>                            // any_of
#include <cassert>                              // assert
#include <functional>                           // logical_or
#include <type_traits>                          // bool_constant

namespace dctl::core::model {
namespace detail {

template<class...>
class king_jumps;

template<class Rules, class Board, color Side>
class king_jumps<Rules, Board, color_<Side>>
{
        using rules_type = Rules;
        using board_type = Board;
        using   set_type = set_t<Board>;

        constexpr static auto king_jump_directions = king_jump_directions_v<rules_type>;

public:
        static auto detect(set_type const& kings, set_type const& targets, set_type const& empty) noexcept
        {
                return std::any_of(kings.begin(), kings.end(), [&](auto from_sq) {
                        return boost::hana::fold(
                                boost::hana::transform(king_jump_directions, [&](auto const dir) {
                                        using direction_t = decltype(dir);
                                        if constexpr (is_long_ranged_king_v<rules_type>) {
                                                auto const blockers = king_jump<rules_type, board_type, direction_t>(from_sq, empty);
                                                if (blockers.empty()) { return false; }
                                                auto const first = find_first<direction_t>(blockers);
                                                return jump_targets<board_type, direction_t>{}(targets, empty).contains(first);
                                        } else {
                                                return jump_sources<board_type, direction_t>{}(targets, empty).contains(from_sq);
                                        }
                                }),
                                std::logical_or{}
                        );
                });
        }

        template<class Builder>
        static auto generate(Builder& b)
        {
                raii::set_king_jump g1{b};
                for (auto const from_sq : b.pieces(color_c<Side>, kings_c)) {
                        raii::lift guard{from_sq, b};
                        boost::hana::for_each(king_jump_directions, [&](auto const dir) {
                                using direction_t = decltype(dir);
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const blockers = king_jump<rules_type, board_type, direction_t>(from_sq, b.pieces(empty_c));
                                        if (blockers.empty()) { return; }
                                        auto const first = find_first<direction_t>(blockers);
                                        if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return; }
                                        capture<direction_t>(next<board_type, direction_t>{}(first), b);
                                } else {
                                        if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(from_sq)) { return; }
                                        capture<direction_t>(next<board_type, direction_t, 2>{}(from_sq), b);
                                }
                        });
                }
        }

        template<class Direction, class Builder>
        static auto next_target_passing_promotion(int const sq, Builder& b)
        {
                constexpr static auto hana_is_forward = [](auto const arg) {
                        return boost::hana::decltype_(arg) == boost::hana::int_c<Direction::value>;
                };

                constexpr static auto hana_is_reverse = [](auto const arg) {
                        return boost::hana::decltype_(arg) == boost::hana::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                };

                constexpr static auto hana_is_forward_or_reverse = [](auto const arg) {
                        return boost::hana::or_(hana_is_forward(arg), hana_is_reverse(arg));
                };

                constexpr auto hana_king_turn_directions = boost::hana::remove_if(king_jump_directions, hana_is_forward_or_reverse);

                static_assert(is_passing_promotion_v<rules_type>);
                assert(b.with() == piece::pawns);
                b.into(piece::kings);
                return scan(hana_king_turn_directions, sq, b);
        }
private:
        template<class Direction, class Builder>
        static auto capture(int const sq, Builder& b)
                -> void
        {
                raii::capture guard{prev<board_type, Direction>{}(sq), b};
                auto const n [[maybe_unused]] = king_slide<rules_type, board_type, Direction>(prev<board_type, Direction>{}(sq), b.pieces(empty_c)).size();
                if (!next_target<Direction>(sq, n, b)) {
                        if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                                if (b.is_last_jumped_king(prev<board_type, Direction>{}(sq))) {
                                        return b.finalize(sq);
                                } else {
                                        return add_sliding_jumps<Direction>(sq, n, b);
                                }
                        }
                        if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                                return add_sliding_jumps<Direction>(sq, n, b);
                        }
                        if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                                return b.finalize(sq);
                        }
                }
        }

        template<class Direction, class Builder>
        static auto add_sliding_jumps(int sq, int n, Builder& b)
        {
                do {
                        b.finalize(sq);
                        advance<board_type, Direction>{}(sq);
                } while (--n);
        }

        template<class Direction, class Builder>
        static auto next_target(int sq, int n [[maybe_unused]], Builder& b)
        {
                constexpr static auto hana_is_forward = [](auto const arg) {
                        return boost::hana::decltype_(arg) == boost::hana::int_c<Direction::value>;
                };

                constexpr static auto hana_is_reverse = [](auto const arg) {
                        return boost::hana::decltype_(arg) == boost::hana::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                };

                constexpr static auto hana_is_forward_or_reverse = [](auto const arg) {
                        return boost::hana::or_(hana_is_forward(arg), hana_is_reverse(arg));
                };

                constexpr auto hana_king_scan_directions [[maybe_unused]] = boost::hana::remove_if(king_jump_directions, hana_is_reverse);
                constexpr auto hana_king_turn_directions [[maybe_unused]] = boost::hana::remove_if(king_jump_directions, hana_is_forward_or_reverse);

                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan(king_jump_directions, sq, b);
                } else if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan(hana_king_scan_directions, sq, b);
                } else {
                        auto found_next = scan(hana_king_scan_directions, sq, b);
                        while (--n) {
                                advance<board_type, Direction>{}(sq);
                                found_next |= scan(hana_king_turn_directions, sq, b);
                        }
                        return found_next;
                }
        }

        template<class Directions, class Builder>
        static auto scan(Directions directions, int const sq, Builder& b)
        {
                return boost::hana::fold(
                        boost::hana::transform(directions, [&](auto const dir) {
                                using direction_t = decltype(dir);
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const blockers = king_jump<rules_type, board_type, direction_t>(sq, b.pieces(empty_c));
                                        if (blockers.empty()) { return false; }
                                        auto const first = find_first<direction_t>(blockers);
                                        if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return false; }
                                        capture<direction_t>(next<board_type, direction_t>{}(first), b);
                                } else {
                                        if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(sq)) { return false; }
                                        capture<direction_t>(next<board_type, direction_t, 2>{}(sq), b);
                                }
                                return true;
                        }),
                        std::bit_or{}
                );                
        }
};

}       // namespace detail
}       // namespace dctl::core::model
