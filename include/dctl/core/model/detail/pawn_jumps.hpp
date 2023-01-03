#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>                    // angle, deg
#include <dctl/core/board/bearing.hpp>                  // bearing
#include <dctl/core/board/mask.hpp>                     // basic_mask
#include <dctl/core/model/detail/builder.hpp>           // builder
#include <dctl/core/model/detail/king_jumps.hpp>        // promote_en_passant
#include <dctl/core/model/detail/pattern.hpp>           // jump_from, jump_targets
#include <dctl/core/model/detail/raii.hpp>              // capture, lift
#include <dctl/core/rules/type_traits.hpp>              // is_superior_rank_jump_t, is_promotion_en_passant_t
#include <dctl/core/state/color.hpp>                    // color, color_
#include <dctl/core/state/piece.hpp>                    // pawn_, king_
#include <dctl/util/type_traits.hpp>                    // action_t, board_t, rules_t, set_t
#include <tabula/tuple.hpp>
#include <cassert>                                      // assert
#include <functional>                                   // bit_or, logical_or
#include <iterator>                                     // next
#include <type_traits>                                  // bool_constant

namespace dctl::core::detail {

template<class...>
class pawn_jumps;

template<class Rules, class Board, color Side>
class pawn_jumps<Rules, Board, color_<Side>>
{
        using rules_type = Rules;
        using board_type = Board;
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;
        using king_jumps = detail::king_jumps<rules_type, board_type, color_<Side>>;

        static constexpr auto pawn_jump_directions = tabula::transform(pawn_jump_directions_v<rules_type>, [](auto dir) {
                return tabula::int_c<rotate(angle{dir}, bearing_v<board_type, color_<Side>>).value()>;
        });

public:
        [[nodiscard]] static constexpr auto detect(set_type const& pawns, set_type const& targets, set_type const& empty) noexcept
        {
                return tabula::any_of(pawn_jump_directions, [&](auto dir) {
                        return !jump_targets<board_type, decltype(dir)>{}(pawns, targets, empty).empty();
                });
        }

        [[nodiscard]] static constexpr auto generate(auto& builder) noexcept
        {
                builder.with(piece::pawn);
                raii::king_targets guard1{&builder};
                tabula::for_each(pawn_jump_directions, [&](auto dir) {
                        using direction_t = decltype(dir);
                        for (auto from_sq : jump_from<board_type, direction_t>{}(builder.pieces(color_c<Side>, pawn_c), builder.targets(), builder.pieces(empty_c))) {
                                raii::lift guard2{from_sq, builder};
                                capture<direction_t>(next<board_type, direction_t, 2>{}(from_sq), builder);
                        }
                });
        }
private:
        template<class Direction>
        static constexpr auto capture(int sq, auto& builder) noexcept
                -> void
        {
                raii::capture guard1{prev<board_type, Direction>{}(sq), builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (mask_type::promotion(Side).contains(sq)) {
                                raii::king_targets guard2{&builder};
                                if (!king_jumps::template next_target_passing_promotion<Direction>(sq, builder)) {
                                        builder.finalize(sq, piece::king);
                                }
                        } else  {
                                if (next_target<Direction>(sq, builder)) { return; }
                                builder.finalize(sq, piece::pawn);
                        }
                } else {
                        if (next_target<Direction>(sq, builder)) { return; }
                        builder.finalize(sq, mask_type::promotion(Side).contains(sq) ? piece::king : piece::pawn);
                }
        }

        template<class Direction>
        [[nodiscard]] static constexpr auto next_target(int sq, auto& builder) noexcept
        {
                constexpr auto is_reverse = [](auto dir) {
                        return dir == tabula::int_c<rotate(angle{Direction::value}, 180_deg).value()>;
                };

                return tabula::any_of_all(tabula::remove_if(pawn_jump_directions, is_reverse), [&](auto dir) {
                        using direction_t = decltype(dir);
                        if (!jump_from<board_type, direction_t>{}(builder.targets(), builder.pieces(empty_c)).contains(sq)) { return false; }
                        capture<direction_t>(next<board_type, direction_t, 2>{}(sq), builder);
                        return true;
                });
        }
};

}       // namespace dctl::core::detail
