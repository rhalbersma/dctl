#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/stride.hpp>  // find_first, advance, next
#include <dctl/util/meta.hpp>                   // make_array, foldl_bit_or, foldl_comma
#include <dctl/util/type_traits.hpp>            // set_t
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <type_traits>                          // bool_constant

namespace dctl::core {
namespace detail {

template<class Board, int Direction, bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
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
                auto result = std::array<std::array<set_t<Board>, meta::size<Directions>::value>, Board::bits()>{};
                Board::squares.for_each([&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::make_array<Directions>{}([=](auto direction) {
                                        constexpr auto direction_v = decltype(direction){};
                                        return scan<Board, direction_v, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, Board::squares);
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
                return meta::make_array<Directions>{}([](auto direction) {
                        constexpr auto direction_v [[maybe_unused]] = decltype(direction){}; // silence bogus GCC warning unused-but-set-variable
                        auto result = std::array<set_t<Board>, Board::bits()>{};
                        Board::squares.for_each([&](auto const sq) {
                                result[static_cast<std::size_t>(sq)] =
                                        scan<Board, direction_v, IsLongRanged, IncludesFrom, IncludesEdge>{}(sq, Board::squares)
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

template<class Rules, class Board>
using basic_king_move_scan = detail::board_scan_sq_dir<Board, basic_king_move_directions, is_long_ranged_king_v<Rules>, false, true>;

template<class Rules, class Board>
using basic_king_jump_scan = detail::board_scan_sq_dir<Board, basic_king_jump_directions<Rules>, is_long_ranged_king_v<Rules>, false, false>;

template<class Rules, class Board>
using basic_blocker_and_beyond = detail::board_scan_dir_sq<Board, basic_king_jump_directions<Rules>, true, true, true>;

constexpr auto move_index(int direction) noexcept
{
        return (direction - 45) / 90;
}

template<class Rules>
constexpr auto jump_index(int direction) noexcept
{
        if constexpr (is_orthogonal_jump_v<Rules>) {
                return direction / 45;
        } else {
                return (direction - 45) / 90;
        }
}

}       // namespace detail

template<class Rules, class Board, int Direction>
auto king_move_scan(int const sq)
{
        constexpr auto index = detail::move_index(Direction);
        return detail::basic_king_move_scan<Rules, Board>{}(sq, index);
}

template<class Rules, class Board, int Direction>
auto king_jump_scan(int const sq)
{
        constexpr auto index = detail::jump_index<Rules>(Direction);
        return detail::basic_king_jump_scan<Rules, Board>{}(sq, index);
}

template<class Rules, class Board, int Direction>
auto blocker_and_beyond(int const sq)
{
        constexpr auto index = detail::jump_index<Rules>(Direction);
        return detail::basic_blocker_and_beyond<Rules, Board>{}(sq, index);
}

template<class Rules, class Board, int... Direction>
class king_moves;

template<class Rules, class Board, int Direction>
class king_moves<Rules, Board, Direction>
{
        using set_type = set_t<Board>;
public:
        auto operator()(int const sq) const
        {
                assert(Board::is_onboard(sq));
                return king_move_scan<Rules, Board, Direction>(sq);
        }

        auto operator()(int const sq, set_type const& empty) const
        {
                assert(Board::is_onboard(sq));
                if constexpr (is_long_ranged_king_v<Rules>) {
                        auto targets = king_move_scan<Rules, Board, Direction>(sq);
                        if (auto const blockers = targets - empty; !blockers.empty()) {
                                targets ^= blocker_and_beyond<Rules, Board, Direction>(find_first<Direction>(blockers));
                        }
                        return targets;
                } else {
                        return king_move_scan<Rules, Board, Direction>(sq) & empty;
                }
        }
};

template<class Rules, class Board>
class king_moves<Rules, Board>
{
        using set_type = set_t<Board>;

        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                Board::squares.for_each([&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::foldl_bit_or<basic_king_move_directions>{}([&](auto direction) {
                                        constexpr auto direction_v = decltype(direction){};
                                        return king_move_scan<Rules, Board, direction_v>(sq);
                                });
                        ;
                });
                return result;
        }();
public:
        auto operator()(int const sq) const
        {
                assert(Board::is_onboard(sq));
                return table[static_cast<std::size_t>(sq)];
        }

        auto operator()(int const sq, set_type const& empty) const
        {
                assert(Board::is_onboard(sq));
                if constexpr (is_long_ranged_king_v<Rules>) {
                        auto targets = table[static_cast<std::size_t>(sq)];
                        meta::foldl_comma<basic_king_move_directions>{}([&](auto direction) {
                                constexpr auto direction_v = decltype(direction){};
                                auto const blockers = king_move_scan<Rules, Board, direction_v>(sq) - empty;
                                if (blockers.empty()) { return; }
                                targets ^= blocker_and_beyond<Rules, Board, direction_v>(find_first<direction_v>(blockers));
                        });
                        return targets;
                } else {
                        return table[static_cast<std::size_t>(sq)] & empty;
                }
        }
};

}       // namespace dctl::core
