#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/builder.hpp>         // builder
#include <dctl/core/actions/detail/king_jump.hpp>       // promote_en_passant
#include <dctl/core/actions/detail/pattern.hpp>         // jump_sources, jump_targets
#include <dctl/core/actions/detail/raii.hpp>            // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/actions/detail/tables.hpp>          // pawn_jump_scan
#include <dctl/core/actions/select/jump.hpp>            // jumps
#include <dctl/core/board/angle.hpp>                    // rotate, inverse
#include <dctl/core/board/bearing.hpp>                  // bearing
#include <dctl/core/state/color_piece.hpp>              // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>              // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/meta.hpp>                           // foldl_logical_or, foldl_comma, foldl_bit_or
#include <dctl/util/type_traits.hpp>                    // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // next
#include <type_traits>                                  // bool_constant

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
        using orient = meta::integral_c<int, rotate(angle{Arg::value}, angle{orientation}).value()>;

        using pawn_jump_directions = meta::transform<orient, basic_pawn_jump_directions<rules_type>>;

        template<class Arg, class Direction>
        using is_reverse = std::bool_constant<Arg::value == rotate_v<Direction::value, 180>>;

        template<int Direction>
        using pawn_scan_directions = meta::remove_if_q<pawn_jump_directions, meta::bind_back<is_reverse, meta::integral_c<int, Direction>>>;
public:
        static auto detect(State const& s) noexcept
        {
                auto const pawns = s.pieces(color_c<Side>, pawns_c);
                if (pawns.empty()) {
                        return false;
                }
                return meta::foldl_logical_or<pawn_jump_directions>{}([&, targets = s.targets(color_c<Side>, pawns_c), empty = s.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        return !jump_targets<board_type, direction_v>{}(pawns, targets, empty)
                                .empty()
                        ;
                });
        }

        template<class Builder>
        static auto generate(Builder& m_builder)
        {
                auto const pawns = m_builder.pieces(color_c<Side>, pawns_c);
                if (pawns.empty()) {
                        return;
                }
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        m_builder.toggle_king_targets();
                }
                meta::foldl_comma<pawn_jump_directions>{}([&, targets = m_builder.targets(), empty = m_builder.pieces(empty_c)](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        jump_sources<board_type, direction_v>{}(pawns, targets, empty)
                                .consume([&](auto from_sq) {
                                        raii::launch<Builder> guard{m_builder, from_sq};
                                        capture<direction_v>(next<board_type, direction_v>{}(from_sq), m_builder);
                                })
                        ;
                });
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        m_builder.toggle_king_targets();
                }
        }
private:
        template<int Direction, class Builder>
        static auto capture(int jumper, Builder& m_builder)
                -> void
        {
                raii::capture<Builder> guard{m_builder, jumper};
                advance<board_type, Direction>{}(jumper);
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (board_type::promotion(Side).contains(jumper)) {
                                return on_promotion<Direction>(jumper, m_builder);
                        }
                        if (next_target<Direction>(jumper, m_builder)) {
                                return;
                        }
                } else {
                        if (next_target<Direction>(jumper, m_builder)) {
                                return;
                        }
                        if (board_type::promotion(Side).contains(jumper)) {
                                return on_promotion<Direction>(jumper, m_builder);
                        }
                }
                m_builder.finalize(jumper);
        }

        template<int Direction, class Builder>
        static auto on_promotion(int jumper, Builder& m_builder)
        {
                raii::promotion<Builder> guard{m_builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if constexpr (is_superior_rank_jump_v<rules_type>) {
                                m_builder.toggle_king_targets();
                        }
                        king_jumps::template try_next_passing_promotion<Direction>(jumper, m_builder);
                        if constexpr (is_superior_rank_jump_v<rules_type>) {
                                m_builder.toggle_king_targets();
                        }
                } else {
                        return m_builder.finalize(jumper);
                }
        }

        template<int Direction, class Builder>
        static auto next_target(int jumper, Builder& m_builder)
        {
                return meta::foldl_bit_or<pawn_scan_directions<Direction>>{}([&](auto direction) {
                        constexpr auto direction_v = decltype(direction){};
                        if (!(pawn_jump_scan<rules_type, board_type, direction_v>(jumper) & m_builder.template targets<direction_v>()).empty()) {
                                capture<direction_v>(next<board_type, direction_v>{}(jumper), m_builder);
                                return true;
                        }
                        return false;
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
