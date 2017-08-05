#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/builder.hpp>         // builder
#include <dctl/core/actions/detail/king_jump.hpp>       // promote_en_passant
#include <dctl/core/actions/detail/raii.hpp>            // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/actions/select/jump.hpp>            // jumps
#include <dctl/core/board/angle.hpp>                    // rotate, inverse
#include <dctl/core/board/bearing.hpp>                  // bearing
#include <dctl/core/board/jump_sources.hpp>             // jump_sources
#include <dctl/core/board/jump_targets.hpp>             // jump_targets
#include <dctl/core/board/ray.hpp>                      // make_iterator, rotate, mirror, turn
#include <dctl/core/state/color_piece.hpp>              // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>              // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/meta.hpp>                           // foldl_logical_or, foldl_comma, foldl_bit_or
#include <dctl/util/type_traits.hpp>                    // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // next
#include <type_traits>                                  // is_same

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
public:
        static auto detect(State const& s) noexcept
        {
                if (auto const pawns = s.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        return meta::foldl_logical_or<pawn_jump_directions<rules_type, orientation>>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                return !jump_targets<board_type, Direction, short_ranged_tag>{}(pawns, s.targets(color_c<Side>, pawns_c), s.pieces(empty_c)).empty();
                        });
                }
                return false;
        }

        template<class Builder>
        static auto generate(Builder& m_builder)
        {
                if (auto const pawns = m_builder.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        if constexpr (is_superior_rank_jump_v<rules_type>) { m_builder.toggle_king_targets(); }
                        meta::foldl_comma<pawn_jump_directions<rules_type, orientation>>{}([&](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                jump_sources<board_type, Direction, short_ranged_tag>{}(pawns, m_builder.targets(), m_builder.pieces(empty_c)).consume([&](auto from_sq) {
                                        raii::launch<Builder> guard{m_builder, from_sq};
                                        capture(std::next(ray::make_iterator<board_type, Direction>(from_sq)), m_builder);
                                });
                        });
                        if constexpr (is_superior_rank_jump_v<rules_type>) { m_builder.toggle_king_targets(); }
                }
        }
private:
        template<class Iterator, class Builder>
        static auto capture(Iterator jumper, Builder& m_builder)
                -> void
        {
                raii::capture<Builder> guard{m_builder, *jumper};
                ++jumper;
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (board_type::promotion(Side).contains(*jumper)) {
                                return on_promotion(jumper, m_builder);
                        }
                        if (next_target(jumper, m_builder)) {
                                return;
                        }
                } else {
                        if (next_target(jumper, m_builder)) {
                                return;
                        }
                        if (board_type::promotion(Side).contains(*jumper)) {
                                return on_promotion(jumper, m_builder);
                        }
                }
                m_builder.finalize(*jumper);
        }

        template<class Iterator, class Builder>
        static auto on_promotion(Iterator jumper, Builder& m_builder)
        {
                raii::promotion<Builder> guard{m_builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        return on_king_jump(jumper, m_builder);
                } else {
                        return m_builder.finalize(*jumper);
                }
        }

        template<class Iterator, class Builder>
        static auto on_king_jump(Iterator jumper, Builder& m_builder)
        {
                static_assert(is_passing_promotion_v<rules_type>);
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        king_jumps::try_next_passing_promotion(jumper, m_builder);
                } else {
                        king_jumps::try_next_passing_promotion(jumper, m_builder);
                }
        }

        template<class Iterator, class Builder>
        static auto next_target(Iterator jumper, Builder& m_builder)
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return meta::foldl_bit_or<pawn_jump_directions<rules_type, orientation>>{}([&](auto direction) {
                        if constexpr (
                                decltype(direction){} == rotate_v<ray::direction_v<Iterator>.value(), 180>
                        ) { return false; }
                        auto const turner = ray::turn<decltype(direction){}>(jumper);
                        if (m_builder.has_pawn_target(turner)) {
                                capture(std::next(turner), m_builder);
                                return true;
                        }
                        return false;
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
