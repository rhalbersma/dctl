#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/bearing.hpp>                  // bearing
#include <dctl/core/state/color_piece.hpp>              // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>              // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/meta.hpp>                           // foldl_logical_or, foldl_comma, foldl_bit_or
#include <dctl/util/type_traits.hpp>                    // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // next
#include <type_traits>                                  // bool_constant
#include <dctl/core/model/builder.hpp>         // builder
#include <dctl/core/model/king_jump.hpp>       // promote_en_passant
#include <dctl/core/model/pattern.hpp>         // jump_targets
#include <dctl/core/model/raii.hpp>            // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/model/select/jump.hpp>            // jumps

namespace dctl::core {
namespace detail {

template<class...>
class pawn_jump;

template<color Side, class Reverse, class State>
class pawn_jump<color_<Side>, Reverse, State>
{
        using  king_jumps = king_jump<color_<Side>, Reverse, State>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;

        template<class Arg>
        using oriented = meta::integral_c<int, rotate_v<Arg::value, orientation>>;

        using pawn_jump_directions = meta::transform<oriented, basic_pawn_jump_directions<rules_type>>;

        template<class Arg, class Direction>
        using is_reverse = std::bool_constant<Arg::value == rotate_v<Direction::value, 180>>;

        template<class Direction>
        using pawn_scan_directions = meta::remove_if_q<pawn_jump_directions, meta::bind_back<is_reverse, Direction>>;
public:
        static auto detect(State const& s) noexcept
        {
                return meta::foldl_logical_or<pawn_jump_directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        return !jump_targets<board_type, direction_t>{}(s.pieces(color_c<Side>, pawns_c), s.targets(color_c<Side>, pawns_c), s.pieces(empty_c)).empty();
                });
        }

        template<class Builder>
        static auto generate(Builder& b)
        {
                if constexpr (is_superior_rank_jump_v<rules_type>) { b.toggle_king_targets(); }
                meta::foldl_comma<pawn_jump_directions>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        jump_sources<board_type, direction_t>{}(b.pieces(color_c<Side>, pawns_c), b.targets(), b.pieces(empty_c)).for_each([&](auto const from_sq) {
                                raii::lift<Builder> guard{from_sq, b};
                                capture<direction_t>(next<board_type, direction_t, 2>{}(from_sq), b);
                        });
                });
                if constexpr (is_superior_rank_jump_v<rules_type>) { b.toggle_king_targets(); }
        }
private:
        template<class Direction, class Builder>
        static auto capture(int const sq, Builder& b)
                -> void
        {
                raii::capture<Builder> guard{prev<board_type, Direction>{}(sq), b};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (board_type::promotion(Side).contains(sq)) {
                                if constexpr (is_superior_rank_jump_v<rules_type>) { b.toggle_king_targets(); }
                                if (!king_jumps::template next_target_passing_promotion<Direction>(sq, b)) {
                                        b.finalize(sq, piece::kings);
                                }
                                if constexpr (is_superior_rank_jump_v<rules_type>) { b.toggle_king_targets(); }
                        } else  {
                                if (next_target<Direction>(sq, b)) { return; }
                                b.finalize(sq, piece::pawns);
                        }
                } else {
                        if (next_target<Direction>(sq, b)) { return; }
                        b.finalize(sq, board_type::promotion(Side).contains(sq) ? piece::kings : piece::pawns);
                }
        }

        template<class Direction, class Builder>
        static auto next_target(int const sq, Builder& b)
        {
                return meta::foldl_bit_or<pawn_scan_directions<Direction>>{}([&](auto const dir) {
                        using direction_t = decltype(dir);
                        if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(sq)) { return false; }
                        capture<direction_t>(next<board_type, direction_t, 2>{}(sq), b);
                        return true;
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
