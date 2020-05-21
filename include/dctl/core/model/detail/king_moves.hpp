#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/model/detail/stride.hpp>    // find_first
#include <dctl/core/model/detail/tables.hpp>    // board_scan_sq_dir, board_scan_dir_sq, move_index
#include <dctl/core/rules/type_traits.hpp>      // is_long_ranged_king_v, king_move_directions
#include <dctl/util/type_traits.hpp>            // set_t
#include <tabula/tuple.hpp>                     // any_of_all
#include <algorithm>                            // any_of
#include <array>                                // array
#include <cstddef>                              // size_t
#include <functional>                           // bit_or
#include <numeric>                              // accumulate

namespace dctl::core::model {
namespace detail {

template<class Rules, class Board>
class king_moves
{
        using mask_type = basic_mask<Board>;
        using  set_type = set_t<mask_type>;

        constexpr static auto king_move_directions = king_move_directions_v<Rules>;

        using king_move_directions_t = std::decay_t<decltype(king_move_directions)>;
        using basic_king_move_scan = board_scan_sq_dir<Board, king_move_directions_t, is_long_ranged_king_v<Rules>, false, true>;
        using basic_blocker_and_beyond = board_scan_dir_sq<Board, king_move_directions_t, true, true, true>;

        template<class Direction>
        static auto king_move_scan(int from_sq)
        {
                assert(Board::is_onboard(from_sq));
                return basic_king_move_scan{}(from_sq, move_index<Direction>);
        }

        template<class Direction>
        static auto blocker_and_beyond(int from_sq)
        {
                assert(Board::is_onboard(from_sq));
                static_assert(is_long_ranged_king_v<Rules>);
                return basic_blocker_and_beyond{}(from_sq, move_index<Direction>);
        }

        inline const static auto attacks_table = []() {
                std::array<set_type, Board::bits()> result;
                for (auto from_sq : mask_type::squares) {
                        result[static_cast<std::size_t>(from_sq)] =
                                tabula::any_of_all(king_move_directions, [&](auto dir) {
                                        return king_move_scan<decltype(dir)>(from_sq);
                                })
                        ;
                }
                return result;
        }();

        // Classical Approach In One Run
        // https://chessprogramming.wikispaces.com/Classical+Approach#Piece%20Attacks-In%20one%20Run
        static auto attacks(int from_sq, set_type const& empty) // Throws: Nothing.
        {
                assert(Board::is_onboard(from_sq));
                if constexpr (is_long_ranged_king_v<Rules>) {
                        return
                                attacks_table[static_cast<std::size_t>(from_sq)] ^
                                tabula::any_of_all(king_move_directions, [&](auto dir) {
                                        using direction_t = decltype(dir);
                                        auto const blockers = king_move_scan<direction_t>(from_sq) - empty;
                                        return blockers.empty() ? blockers : blocker_and_beyond<direction_t>(find_first<direction_t>(blockers));
                                })
                        ;
                } else {
                        return attacks_table[static_cast<std::size_t>(from_sq)] & empty;
                }
        }
public:
        static auto detect(set_type const& kings, set_type const& empty) noexcept
        {
                return std::any_of(kings.begin(), kings.end(), [&](auto from_sq) {
                        return !attacks(from_sq, empty).empty();
                });
        }

        static auto count(set_type const& kings, set_type const& empty) noexcept
        {
                return std::accumulate(kings.begin(), kings.end(), 0, [&](auto result, auto from_sq) {
                        return result + attacks(from_sq, empty).ssize();
                });
        }

        template<class SequenceContainer>
        static auto generate(set_type const& kings, set_type const& empty, SequenceContainer& seq)
        {
                for (auto from_sq : kings) {
                        for (auto dest_sq : attacks(from_sq, empty)) {
                                seq.emplace_back(from_sq, dest_sq);
                        }
                }
        }
};

}       // namespace detail
}       // namespace dctl::core::model
