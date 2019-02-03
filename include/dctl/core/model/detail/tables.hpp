#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/model/detail/stride.hpp>    // find_first, advance, next
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/array.hpp>                       // array_from_types
#include <boost/hana/size.hpp>                  // size
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <type_traits>                          // bool_constant

namespace dctl::core::model {
namespace detail {

template<class Tuple>
constexpr auto tuple_size = decltype(boost::hana::size(std::declval<Tuple>()))::value;

template<class Board, class Direction, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
struct scan
{
        using mask_type = basic_mask<Board>;
        using  set_type = set_t<mask_type>;

        auto operator()(int from, set_type const& propagator) const
        {
                assert(Board::is_onboard(from));
                auto const is_within = [&](auto sq) {
                        return Board::is_onboard(sq) && propagator.contains(sq);
                };
                auto const is_valid = [&](auto sq) {
                        return is_within(sq) && (IncludesEdge || is_within(next<Board, Direction>{}(sq)));
                };
                if constexpr (!IncludesFrom) {
                        advance<Board, Direction>{}(from);
                }
                set_type targets;
                if constexpr (IsLongRanged) {
                        while (is_valid(from)) {
                                targets.insert(from);
                                advance<Board, Direction>{}(from);
                        }
                } else {
                        if (is_valid(from)) {
                                targets.insert(from);
                        }
                }
                return targets;
        }
};

template<class Board, class Directions, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
class board_scan_sq_dir
{
        inline const static auto table = []() {
                using mask_type = basic_mask<Board>;
                using  set_type = set_t<mask_type>;
                std::array<std::array<set_type, tuple_size<Directions>>, Board::bits()> result;
                for (auto sq : mask_type::squares) {
                        result[static_cast<std::size_t>(sq)] =
                                xstd::array_from_types<Directions>{}([=](auto dir) {
                                        using direction_t = decltype(dir);
                                        return scan<Board, direction_t, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, mask_type::squares);
                                })
                        ;
                }
                return result;
        }();
public:
        auto operator()(int sq, int index) const
        {
                return table[static_cast<std::size_t>(sq)][static_cast<std::size_t>(index)];
        }
};

template<class Board, class Directions, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
class board_scan_dir_sq
{
        inline const static auto table = []() {
                return xstd::array_from_types<Directions>{}([](auto dir) {
                        using direction_t = decltype(dir);
                        using mask_type = basic_mask<Board>;
                        using  set_type = set_t<mask_type>;
                        std::array<set_type, Board::bits()> result;
                        for (auto sq : mask_type::squares) {
                                result[static_cast<std::size_t>(sq)] =
                                        scan<Board, direction_t, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, mask_type::squares)
                                ;
                        }
                        return result;
                });
        }();
public:
        auto operator()(int sq, int index) const
        {
                return table[static_cast<std::size_t>(index)][static_cast<std::size_t>(sq)];
        }
};

template<class Rules>
using king_jump_directions_t = std::decay_t<decltype(king_jump_directions_v<Rules>)>;

template<class Rules, class Board>
using basic_king_slide = board_scan_sq_dir<Board, king_jump_directions_t<Rules>, is_long_ranged_king_v<Rules>, false, true>;

template<class Rules, class Board>
using basic_king_jump = board_scan_sq_dir<Board, king_jump_directions_t<Rules>, is_long_ranged_king_v<Rules>, false, false>;

template<class Rules, class Board>
using basic_blocker_and_beyond = board_scan_dir_sq<Board, king_jump_directions_t<Rules>, true, true, true>;

template<class Direction>
constexpr auto move_index = (Direction::value - 45) / 90;

template<class Rules, class Direction>
constexpr auto jump_index = is_orthogonal_jump_v<Rules> ? Direction::value / 45 : move_index<Direction>;

template<class Rules, class Board, class Direction>
auto king_slide(int sq)
{
        return basic_king_slide<Rules, Board>{}(sq, jump_index<Rules, Direction>);
}

template<class Rules, class Board, class Direction>
auto blocker_and_beyond(int sq)
{
        static_assert(is_long_ranged_king_v<Rules>);
        return basic_blocker_and_beyond<Rules, Board>{}(sq, jump_index<Rules, Direction>);
}

template<class Rules, class Board, class Direction, class Set>
auto king_slide(int sq, Set const& empty)
{
        assert(Board::is_onboard(sq));
        if constexpr (is_long_ranged_king_v<Rules>) {
                auto targets = king_slide<Rules, Board, Direction>(sq);
                if (auto const blockers = targets - empty; !blockers.empty()) {
                        targets ^= blocker_and_beyond<Rules, Board, Direction>(find_first<Direction>(blockers));
                }
                return targets;
        } else {
                return king_slide<Rules, Board, Direction>(sq) & empty;
        }
}

template<class Rules, class Board, class Direction, class Set>
auto king_jump(int sq, Set const& empty)
{
        return basic_king_jump<Rules, Board>{}(sq, jump_index<Rules, Direction>) - empty;
}

}       // namespace detail
}       // namespace dctl::core::model
