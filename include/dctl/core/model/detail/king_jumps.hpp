#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/model/detail/builder.hpp>   // builder
#include <dctl/core/model/detail/pattern.hpp>   // jump_from, jump_targets
#include <dctl/core/model/detail/raii.hpp>      // capture, lift, set_king_jump
#include <dctl/core/model/detail/tables.hpp>    // king_jumps, king_moves
#include <dctl/core/rules/type_traits.hpp>      // is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/core/state/color.hpp>            // color, color_
#include <dctl/core/state/piece.hpp>            // king_
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <boost/hana/equal.hpp>                 // ==
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/hana/integral_constant.hpp>     // bool_c, int_c
#include <boost/hana/remove_if.hpp>             // remove_if
#include <algorithm>                            // any_of
#include <cassert>                              // assert
#include <functional>                           // bit_or, logical_or
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
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;

        constexpr static auto king_jump_directions = king_jump_directions_v<rules_type>;
public:
        static auto detect(set_type const& kings, set_type const& targets, set_type const& empty) noexcept
        {
                return std::any_of(kings.begin(), kings.end(), [&](auto from_sq) {
                        return boost::hana::fold(
                                boost::hana::transform(king_jump_directions, [&](auto dir) {
                                        using direction_t = decltype(dir);
                                        if constexpr (is_long_ranged_king_v<rules_type>) {
                                                auto const blockers = king_jump<rules_type, board_type, direction_t>(from_sq, empty);
                                                if (blockers.empty()) { return false; }
                                                auto const first = find_first<direction_t>(blockers);
                                                return jump_targets<board_type, direction_t>{}(targets, empty).contains(first);
                                        } else {
                                                return jump_from<board_type, direction_t>{}(targets, empty).contains(from_sq);
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
                for (auto from_sq : b.pieces(color_c<Side>, king_c)) {
                        raii::lift guard{from_sq, b};
                        boost::hana::for_each(king_jump_directions, [&](auto dir) {
                                using direction_t = decltype(dir);
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const blockers = king_jump<rules_type, board_type, direction_t>(from_sq, b.pieces(empty_c));
                                        if (blockers.empty()) { return; }
                                        auto const first = find_first<direction_t>(blockers);
                                        if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return; }
                                        capture<direction_t>(next<board_type, direction_t>{}(first), b);
                                } else {
                                        if (!jump_from<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(from_sq)) { return; }
                                        capture<direction_t>(next<board_type, direction_t, 2>{}(from_sq), b);
                                }
                        });
                }
        }

        template<class Direction, class Builder>
        static auto next_target_passing_promotion(int sq, Builder& b)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(b.with() == piece::pawn);
                b.into(piece::king);
                constexpr auto king_scan_directions = boost::hana::remove_if(king_jump_directions, [](auto dir) {
                        return boost::hana::bool_c<is_up(rotate(angle{decltype(dir)::value}, bearing_v<board_type, color_<Side>>))>;
                });
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan(king_scan_directions, sq, b);
                } else {
                        constexpr auto is_reverse = [](auto dir) {
                                return dir == boost::hana::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                        };
                        return scan(boost::hana::remove_if(king_scan_directions, is_reverse), sq, b);
                }
        }
private:
        template<class Direction, class Builder>
        static auto capture(int sq, Builder& b)
                -> void
        {
                raii::capture guard{prev<board_type, Direction>{}(sq), b};
                auto const n [[maybe_unused]] = king_slide<rules_type, board_type, Direction>(prev<board_type, Direction>{}(sq), b.pieces(empty_c)).ssize();
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
                constexpr auto king_scan_directions [[maybe_unused]] = boost::hana::remove_if(king_jump_directions, [](auto dir) {
                        return dir == boost::hana::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                });
                constexpr auto king_turn_directions [[maybe_unused]] = boost::hana::remove_if(king_scan_directions, [](auto dir) {
                        return dir == boost::hana::int_c<Direction::value>;
                });
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan(king_jump_directions, sq, b);
                } else if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan(king_scan_directions, sq, b);
                } else {
                        auto found_next = scan(king_scan_directions, sq, b);
                        while (--n) {
                                advance<board_type, Direction>{}(sq);
                                found_next |= scan(king_turn_directions, sq, b);
                        }
                        return found_next;
                }
        }

        template<class Directions, class Builder>
        static auto scan(Directions directions, int sq, Builder& b)
        {
                return boost::hana::fold(
                        boost::hana::transform(directions, [&](auto dir) {
                                using direction_t = decltype(dir);
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const blockers = king_jump<rules_type, board_type, direction_t>(sq, b.pieces(empty_c));
                                        if (blockers.empty()) { return false; }
                                        auto const first = find_first<direction_t>(blockers);
                                        if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return false; }
                                        capture<direction_t>(next<board_type, direction_t>{}(first), b);
                                } else {
                                        if (!jump_from<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(sq)) { return false; }
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
