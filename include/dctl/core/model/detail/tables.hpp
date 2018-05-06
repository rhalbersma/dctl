#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/stride.hpp>    // find_first, advance, next
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/meta.hpp>                   // make_array, foldl_bit_or, foldl_comma
#include <dctl/util/type_traits.hpp>            // set_t
#include <boost/mp11/list.hpp>                  // mp_size
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <type_traits>                          // bool_constant

namespace dctl::core::model {
namespace detail {

template<class Board, class Direction, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
struct scan
{
        using   set_type = set_t<Board>;

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
                auto targets = set_type{};
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
                auto result = std::array<std::array<set_t<Board>, boost::mp11::mp_size<Directions>::value>, Board::bits()>{};
                Board::squares.for_each([&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::make_array<Directions>{}([=](auto const dir) {
                                        using direction_t = decltype(dir);
                                        return scan<Board, direction_t, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, Board::squares);
                                });
                        ;
                });
                return result;
        }();
public:
        auto operator()(int const sq, int const index) const
        {
                return table[static_cast<std::size_t>(sq)][static_cast<std::size_t>(index)];
        }
};

template<class Board, class Directions, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
class board_scan_dir_sq
{
        inline const static auto table = []() {
                return meta::make_array<Directions>{}([](auto const dir) {
                        using direction_t = decltype(dir);
                        auto result = std::array<set_t<Board>, Board::bits()>{};
                        Board::squares.for_each([&](auto const sq) {
                                result[static_cast<std::size_t>(sq)] =
                                        scan<Board, direction_t, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, Board::squares)
                                ;
                        });
                        return result;
                });
        }();
public:
        auto operator()(int const sq, int const index) const
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
auto king_slide(int const sq)
{
        return basic_king_slide<Rules, Board>{}(sq, jump_index<Rules, Direction>);
}

template<class Rules, class Board, class Direction>
auto blocker_and_beyond(int const sq)
{
        static_assert(is_long_ranged_king_v<Rules>);
        return basic_blocker_and_beyond<Rules, Board>{}(sq, jump_index<Rules, Direction>);
}

template<class Rules, class Board, class Direction>
auto king_slide(int const sq, set_t<Board> const& empty)
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

template<class Rules, class Board, class Direction>
auto king_jump(int const sq, set_t<Board> const& empty)
{
        return basic_king_jump<Rules, Board>{}(sq, jump_index<Rules, Direction>) - empty;
}

}       // namespace detail
}       // namespace dctl::core::model
