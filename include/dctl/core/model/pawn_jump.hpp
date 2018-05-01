#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/model/builder.hpp>          // builder
#include <dctl/core/model/king_jump.hpp>        // promote_en_passant
#include <dctl/core/model/pattern.hpp>          // jump_targets
#include <dctl/core/model/raii.hpp>             // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/model/select/jump.hpp>      // jumps
#include <dctl/core/state/color_piece.hpp>      // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>      // is_superior_rank_jump_t, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/type_traits.hpp>            // action_t, board_t, rules_t, set_t
#include <boost/hana/equal.hpp>                 // ==
#include <boost/hana/fold.hpp>                  // fold
#include <boost/hana/for_each.hpp>              // for_each
#include <boost/hana/integral_constant.hpp>     // int_c
#include <boost/hana/remove_if.hpp>             // remove_if
#include <boost/hana/transform.hpp>             // transform
#include <boost/hana/type.hpp>                  // decltype_
#include <cassert>                              // assert
#include <functional>                           // bit_or, logical_or
#include <iterator>                             // next
#include <type_traits>                          // bool_constant

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

        constexpr static auto pawn_jump_directions = boost::hana::transform(pawn_jump_directions_v<rules_type>, [](auto const dir) {
                return boost::hana::int_c<rotate(angle{dir}, angle{orientation}).value()>;
        });
public:
        static auto detect(State const& s) noexcept
        {
                return boost::hana::fold(
                        boost::hana::transform(pawn_jump_directions, [&](auto const dir) {
                                using direction_t = decltype(dir);
                                return !jump_targets<board_type, direction_t>{}(s.pieces(color_c<Side>, pawns_c), s.targets(color_c<Side>, pawns_c), s.pieces(empty_c)).empty();
                        }),
                        std::logical_or{}
                );
        }

        template<class Builder>
        static auto generate(Builder& b)
        {
                if constexpr (is_superior_rank_jump_v<rules_type>) { b.toggle_king_targets(); }
                boost::hana::for_each(pawn_jump_directions, [&](auto const dir) {
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
                constexpr static auto is_reverse = [](auto const arg) {
                        return boost::hana::decltype_(arg) == boost::hana::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                };
 
                return boost::hana::fold(
                        boost::hana::transform(boost::hana::remove_if(pawn_jump_directions, is_reverse), [&](auto const dir) {
                                using direction_t = decltype(dir);
                                if (!jump_sources<board_type, direction_t>{}(b.targets(), b.pieces(empty_c)).contains(sq)) { return false; }
                                capture<direction_t>(next<board_type, direction_t, 2>{}(sq), b);
                                return true;
                        }),
                        std::bit_or{}
                );
        }
};

}       // namespace detail
}       // namespace dctl::core
