#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // is_long_ranged_king_v, king_move_directions
#include <dctl/util/meta.hpp>                   // foldl_bit_or, foldl_comma
#include <dctl/util/type_traits.hpp>            // set_t
#include <array>                                // array
#include <cstddef>                              // size_t
#include <dctl/core/model/stride.hpp>  // find_first
#include <dctl/core/model/tables.hpp>  // board_scan_sq_dir, board_scan_dir_sq, move_index

namespace dctl::core {
namespace detail {

template<class Rules, class Board>
class king_move
{
        using set_type = set_t<Board>;
        using king_move_directions_t = std::decay_t<decltype(king_move_directions_v<Rules>)>;
        using basic_king_move_scan = board_scan_sq_dir<Board, king_move_directions_t, is_long_ranged_king_v<Rules>, false, true>;
        using basic_blocker_and_beyond = board_scan_dir_sq<Board, king_move_directions_t, true, true, true>;

        template<class Direction>
        static auto king_move_scan(int const from_sq)
        {
                assert(Board::is_onboard(from_sq));
                return basic_king_move_scan{}(from_sq, move_index<Direction>);
        }

        template<class Direction>
        static auto blocker_and_beyond(int const from_sq)
        {
                assert(Board::is_onboard(from_sq));
                static_assert(is_long_ranged_king_v<Rules>);
                return basic_blocker_and_beyond{}(from_sq, move_index<Direction>);
        }

        inline const static auto attacks_table = []() {
                auto result = std::array<set_type, Board::bits()>{};
                Board::squares.for_each([&](auto const from_sq) {
                        result[static_cast<std::size_t>(from_sq)] =
                                meta::foldl_bit_or<king_move_directions_t>{}([&](auto const dir) {
                                        using direction_t = decltype(dir);
                                        return king_move_scan<direction_t>(from_sq);
                                });
                        ;
                });
                return result;
        }();

        // Classical Approach In One Run
        // https://chessprogramming.wikispaces.com/Classical+Approach#Piece%20Attacks-In%20one%20Run
        static auto attacks(int const from_sq, set_type const& empty) // Throws: Nothing.
        {
                assert(Board::is_onboard(from_sq));
                if constexpr (is_long_ranged_king_v<Rules>) {
                        return
                                attacks_table[static_cast<std::size_t>(from_sq)] ^
                                meta::foldl_bit_or<king_move_directions_t>{}([&](auto const dir) {
                                        using direction_t = decltype(dir);
                                        auto const blockers = king_move_scan<direction_t>(from_sq) - empty;
                                        return blockers.empty() ? set_type{} : blocker_and_beyond<direction_t>(find_first<direction_t>(blockers));
                                })
                        ;
                } else {
                        return attacks_table[static_cast<std::size_t>(from_sq)] & empty;
                }
        }
public:
        static auto detect(set_type const& kings, set_type const& empty) noexcept
        {
                return kings.any_of([&](auto const from_sq) {
                        return !attacks(from_sq, empty).empty();
                });
        }

        static auto count(set_type const& kings, set_type const& empty) noexcept
        {
                return kings.accumulate(0, [&](auto const result, auto const from_sq) {
                        return result + attacks(from_sq, empty).count();
                });
        }

        template<class SequenceContainer>
        static auto generate(set_type const& kings, set_type const& empty, SequenceContainer& seq)
        {
                kings.for_each([&](auto const from_sq) {
                        attacks(from_sq, empty).for_each([&](auto const dest_sq) {
                                seq.emplace_back(from_sq, dest_sq);
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
