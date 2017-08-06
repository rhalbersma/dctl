#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/shift.hpp>            // first, shift_sign, shift_size
#include <dctl/util/meta.hpp>                   // make_array, foldl_bit_or, foldl_comma
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/int_set.hpp>
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/operators.hpp>                  // totally_ordered, unit_steppable, additive
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // random_access_iterator_tag
#include <type_traits>                          // bool_constant

namespace dctl::core {
namespace ray {

template<class Board, int Direction>
class cursor
:       boost::totally_ordered< cursor<Board, Direction>        // != >= > <=
,       boost::unit_steppable < cursor<Board, Direction>        // ++, --
,       boost::additive       < cursor<Board, Direction>, int   // +, -
> > >
{
        constexpr static auto sign = shift_sign_v<Direction>;
        constexpr static auto N = static_cast<int>(shift_size_v<Board, Direction>);
        constexpr static auto stride = (sign == direction::right) ? -N : N;
        static_assert(stride != 0, "Cursors need a non-zero stride.");

        int m_cursor{};
public:
        cursor() = default;

        explicit cursor(int const sq) noexcept
        :
                m_cursor{sq}
        {}

        template<class, int>
        friend class cursor;

        template<int M>
        /* implicit */ cursor(cursor<Board, M> const& other) noexcept
        :
                m_cursor{other.m_cursor}
        {}

        /* implicit */ operator auto() const noexcept
        {
                return m_cursor;
        }

        // operator++(int) provided by boost::unit_steppable
        auto& operator++() noexcept
        {
                m_cursor += stride;
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        auto& operator--() noexcept
        {
                m_cursor -= stride;
                return *this;
        }

        // operator+(cursor, int) provided by boost::additive
        auto& operator+=(int const n) noexcept
        {
                m_cursor += n * stride;
                return *this;
        }

        // operator-(cursor, int) provided by boost::additive
        auto& operator-=(int const n) noexcept
        {
                m_cursor -= n * stride;
                return *this;
        }

        // number of increments / decrements between lhs and rhs
        friend auto operator-(cursor const& lhs, cursor const& rhs) noexcept
        {
                return (lhs.m_cursor - rhs.m_cursor) / stride;
        }
};

template<class Board, int Direction>
using iterator = boost::counting_iterator
<
        cursor<Board, Direction>,               // Incrementable
        std::random_access_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction>
auto make_iterator(int const sq)
        -> iterator<Board, Direction>
{
        return { cursor<Board, Direction>{sq} };
}

template<int Theta, class Board, int Direction>
auto rotate(iterator<Board, Direction> it)
        -> iterator<Board, rotate(angle{Direction}, angle{Theta}).value()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(iterator<Board, Direction> it)
        -> iterator<Board, mirror(angle{Direction}, angle{Theta}).value()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto turn(iterator<Board, Direction> it)
        -> iterator<Board, Theta>
{
        //static_assert(Theta != Direction);
        return { it.base() };
}

template<class T>
constexpr auto direction_v = angle{};

template<class Board, int Direction>
constexpr auto direction_v<iterator<Board, Direction>> = angle{Direction};

template<bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
struct scan
{
        template<class Board, int Direction, class Set>
        auto operator()(iterator<Board, Direction> from, Set const propagator) const
        {
                assert(Board::is_onboard(*from));
                auto const is_within = [&](auto it) {
                        return Board::is_onboard(*it) && propagator.contains(*it);
                };
                auto const is_valid = [&](auto it) {
                        return is_within(it) && (IncludesEdge || is_within(std::next(it)));
                };
                if constexpr (!IncludesFrom) {
                        ++from;
                }
                auto targets = Set{};
                if constexpr (IsLongRanged) {
                        while (is_valid(from)) {
                                targets.insert(*from++);
                        }
                } else {
                        if (is_valid(from)) {
                                targets.insert(*from);
                        }
                }
                return targets;
        }
};

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

template<class Board, class Scan, class Directions>
class board_scan_sq_dir
{
        inline const static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, meta::size<Directions>::value>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::make_array<Directions>{}([=](auto direction) {
                                        return Scan{}(make_iterator<Board, decltype(direction){}>(sq), Board::squares);
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

template<class Rules, class Board>
using king_move_scan = board_scan_sq_dir<Board, scan<is_long_ranged_king_v<Rules>, false, true>, basic_king_move_directions>;

template<class Rules, class Board>
using king_jump_scan = board_scan_sq_dir<Board, scan<is_long_ranged_king_v<Rules>, false, false>, basic_king_jump_directions<Rules>>;

template<class Rules, class Board>
using pawn_jump_scan = board_scan_sq_dir<Board, scan<false, false, false>, basic_king_jump_directions<Rules>>;

template<class Board, class Scan, class Directions>
class board_scan_dir_sq
{
        inline const static auto table = []() {
                return meta::make_array<Directions>{}([](auto direction) {
                        auto result = std::array<set_t<Board>, Board::bits()>{};
                        xstd::for_each(Board::squares, [&](auto const sq) {
                                result[static_cast<std::size_t>(sq)] = Scan{}(make_iterator<Board, decltype(direction){}>(sq), Board::squares);
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
using blocker_and_beyond = board_scan_dir_sq<Board, scan<true, true, true>, basic_king_jump_directions<Rules>>;

template<class Rules, class Board>
class king_move_scan_diag
{
        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::foldl_bit_or<basic_king_move_directions>{}([=](auto direction) {
                                        return king_move_scan<Rules, Board>{}(sq, move_index(direction));
                                });
                        ;
                });
                return result;
        }();
public:
        auto operator()(int const sq) const
        {
                return table[static_cast<std::size_t>(sq)];
        }
};

template<class Rules, class Board>
class king_moves
{
        using set_type = set_t<Board>;

        template<int Direction>
        auto clear_blocker_and_beyond(int const sq, set_type const& occupied, set_type& targets) const
        {
                if (auto const blockers = king_move_scan<Rules, Board>{}(sq, move_index(Direction)) & occupied; !blockers.empty()) {
                        targets ^= blocker_and_beyond<Rules, Board>{}(find_first<Direction>(blockers), jump_index<Rules>(Direction));
                }
        }
public:
        auto operator()(int const sq, set_type const& occupied) const
        {
                if constexpr (is_long_ranged_king_v<Rules>) {
                        auto targets = king_move_scan_diag<Rules, Board>{}(sq);
                        meta::foldl_comma<basic_king_move_directions>{}([&, this](auto direction) {
                                clear_blocker_and_beyond<decltype(direction){}>(sq, occupied, targets);
                        });
                        return targets;
                } else {
                        return king_move_scan_diag<Rules, Board>{}(sq) - occupied;
                }
        }

        template<class Direction>
        auto ahead(int const sq, set_type const& occupied) const
        {
                if constexpr (is_long_ranged_king_v<Rules>) {
                        auto targets = king_move_scan<Rules, Board>{}(sq, move_index(Direction{}));
                        clear_blocker_and_beyond<Direction{}>(sq, occupied, targets);
                        return targets;
                }
        }
};

template<class Rules, class Board, int Direction>
auto king_jump_target(int sq, set_t<Board> const& targets)
{
        return king_jump_scan<Rules, Board>{}(sq, jump_index<Rules>(Direction)) & targets;
}

template<class Rules, class Board, int Direction>
auto pawn_jump_target(int sq, set_t<Board> const& targets)
{
        return pawn_jump_scan<Rules, Board>{}(sq, jump_index<Rules>(Direction)) & targets;
}

}       // namespace ray
}       // namespace dctl::core
