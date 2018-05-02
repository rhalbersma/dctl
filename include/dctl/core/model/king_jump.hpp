#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/model/builder.hpp>          // builder
#include <dctl/core/model/pattern.hpp>          // jump_targets
#include <dctl/core/model/raii.hpp>             // capture, lift, set_king_jump
#include <dctl/core/model/tables.hpp>           // king_jumps, king_moves
#include <dctl/core/model/select/jump.hpp>      // jump
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_comma, foldl_bit_or
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/mp11/algorithm.hpp>             // mp_remove_if_q, mp_transform
#include <boost/mp11/bind.hpp>                  // mp_bind_back
#include <cassert>                              // assert
#include <functional>                           // logical_or
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

        constexpr static auto king_jump_directions = king_jump_directions_v<rules_type>;

        using king_jump_directions_t = std::decay_t<decltype(king_jump_directions)>;

        template<class Arg, class Direction>
        using is_forward = std::bool_constant<Arg::value == Direction::value>;

        template<class Arg, class Direction>
        using is_reverse = std::bool_constant<Arg::value == rotate_v<Direction::value, 180>>;

        template<class Arg, class Direction>
        using is_forward_or_reverse = std::disjunction<is_forward<Arg, Direction>, is_reverse<Arg, Direction>>;

        template<class Direction>
        using king_scan_directions_t = boost::mp11::mp_remove_if_q<king_jump_directions_t, boost::mp11::mp_bind_back<is_reverse, Direction>>;

        template<class Direction>
        using king_turn_directions_t = boost::mp11::mp_remove_if_q<king_jump_directions_t, boost::mp11::mp_bind_back<is_forward_or_reverse, Direction>>;

        constexpr static auto GCC7_ICE_WORK_AROUND = is_long_ranged_king_v<rules_type>;
public:
        static auto detect(State const& s) noexcept
        {
                return s.pieces(color_c<Side>, kings_c).any_of([&](auto from_sq) {
                        return boost::hana::fold(
                                boost::hana::transform(king_jump_directions, [&](auto const dir) {
                                        using direction_t = decltype(dir);
                                        if constexpr (is_long_ranged_king_v<rules_type>) {
                                                auto const blockers = king_jumps<rules_type, board_type, direction_t>(from_sq, s.pieces(empty_c));
                                                if (blockers.empty()) { return false; }
                                                auto const first = find_first<direction_t>(blockers);
                                                return jump_targets<board_type, direction_t>{}(s.targets(color_c<Side>, kings_c), s.pieces(empty_c)).contains(first);
                                        } else {
                                                return jump_sources<board_type, direction_t>{}(s.targets(color_c<Side>, kings_c), s.pieces(empty_c)).contains(from_sq);
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
                b.pieces(color_c<Side>, kings_c).for_each([&](auto const from_sq) {
                        raii::lift guard{from_sq, b};
                        boost::hana::for_each(king_jump_directions, [&](auto const dir) {
                                using direction_t = decltype(dir);
                                if constexpr (is_long_ranged_king_v<rules_type>) {
                                        auto const blockers = king_jumps<rules_type, board_type, direction_t>(from_sq, b.pieces(empty_c));
                                        if (blockers.empty()) { return; }
                                        auto const first = find_first<direction_t>(blockers);
                                        if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return; }
                                        capture<direction_t>(next<board_type, direction_t>{}(first), b);
                                } else {
                                        if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(from_sq)) { return; }
                                        capture<direction_t>(next<board_type, direction_t, 2>{}(from_sq), b);
                                }
                        });
                });
        }

        template<class Direction, class Builder>
        static auto next_target_passing_promotion(int const sq, Builder& b)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                assert(b.with() == piece::pawns);
                b.into(piece::kings);
                return scan<king_turn_directions_t<Direction>>(sq, b);
        }
private:
        template<class Direction, class Builder>
        static auto capture(int const sq, Builder& b)
                -> void
        {
                raii::capture guard{prev<board_type, Direction>{}(sq), b};
                auto const n [[maybe_unused]] = king_slide<rules_type, board_type, Direction>(prev<board_type, Direction>{}(sq), b.pieces(empty_c)).count();
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
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan<king_jump_directions_t>(sq, b);
                } else if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan<king_scan_directions_t<Direction>>(sq, b);
                } else {
                        auto found_next = scan<king_scan_directions_t<Direction>>(sq, b);
                        while (--n) {
                                advance<board_type, Direction>{}(sq);
                                found_next |= scan<king_turn_directions_t<Direction>>(sq, b);
                        }
                        return found_next;
                }
        }

        template<class Directions, class Builder>
        static auto scan(int const sq, Builder& b)
        {
                return meta::foldl_bit_or<Directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        if constexpr (is_long_ranged_king_v<rules_type>) {
                                auto const blockers = king_jumps<rules_type, board_type, direction_t>(sq, b.pieces(empty_c));
                                if (blockers.empty()) { return false; }
                                auto const first = find_first<direction_t>(blockers);
                                if (!jump_targets<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(first)) { return false; }
                                capture<direction_t>(next<board_type, direction_t>{}(first), b);
                        } else {
                                if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(sq)) { return false; }
                                capture<direction_t>(next<board_type, direction_t, 2>{}(sq), b);
                        }
                        return true;
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
