#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/shift.hpp>     // first, shift_sign, shift_size
#include <dctl/util/type_traits.hpp>            // set_t
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
        static_assert(Theta != Direction);
        return { it.base() };
}

template<class T>
constexpr auto direction_v = angle{};

template<class Board, int Direction>
constexpr auto direction_v<iterator<Board, Direction>> = angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(iterator<Board, Direction> it)
{
        return static_cast<unsigned>(*it) < static_cast<unsigned>(set_t<Board>::max_size());
}

struct exclusive_tag : std:: true_type {};
struct inclusive_tag : std::false_type {};

template<class ExcludesFrom, int LookAhead>
struct fill
{
        template<class Board, int Direction>
        auto operator()(iterator<Board, Direction> from, set_t<Board> const propagator) const
        {
                if constexpr (ExcludesFrom{}) { ++from; }
                auto targets = set_t<Board>{};
                while (is_onboard(from) && is_onboard(std::next(from, LookAhead)) && propagator.contains(*from)) {
                        targets.insert(*from++);
                }
                return targets;
        }
};

template<class Board, class ExcludesFrom, int LookAhead>
class king_targets_sq_dir
{
        inline const static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, 4>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                        {{
                                fill<ExcludesFrom, LookAhead>{}(make_iterator<Board,  45>(sq), Board::squares),
                                fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 135>(sq), Board::squares),
                                fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 225>(sq), Board::squares),
                                fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 315>(sq), Board::squares)
                        }};
                });
                return result;
        }();
public:
        auto operator()(int const sq, int const direction_index) const
        {
                return table[static_cast<std::size_t>(sq)][static_cast<std::size_t>(direction_index)];
        }
};

template<class Board>
using king_moves_empty = king_targets_sq_dir<Board, exclusive_tag, 0>;

template<class Board>
using king_jumps_empty = king_targets_sq_dir<Board, exclusive_tag, 1>;

template<class Board, class ExcludesFrom, int LookAhead>
class king_targets_dir_sq
{
        inline const static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, Board::bits()>, 4>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[0][static_cast<std::size_t>(sq)] = fill<ExcludesFrom, LookAhead>{}(make_iterator<Board,  45>(sq), Board::squares);
                        result[1][static_cast<std::size_t>(sq)] = fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 135>(sq), Board::squares);
                        result[2][static_cast<std::size_t>(sq)] = fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 225>(sq), Board::squares);
                        result[3][static_cast<std::size_t>(sq)] = fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, 315>(sq), Board::squares);
                });
                return result;
        }();
public:
        auto operator()(int const sq, int const direction_index) const
        {
                return table[static_cast<std::size_t>(direction_index)][static_cast<std::size_t>(sq)];
        }
};

template<class Board>
using blocker_and_beyond = king_targets_dir_sq<Board, inclusive_tag, 0>;

template<class Board>
class king_moves_empty_diagonal
{
        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                king_moves_empty<Board>{}(sq, 0) |
                                king_moves_empty<Board>{}(sq, 1) |
                                king_moves_empty<Board>{}(sq, 2) |
                                king_moves_empty<Board>{}(sq, 3)
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

template<class Board, int DirectionIndex>
auto clear_blocker_and_beyond(set_t<Board>& targets, int const sq, set_t<Board> const occupied)
{
        constexpr auto Direction = 45 + 90 * DirectionIndex;
        if (auto const blockers = king_moves_empty<Board>{}(sq, DirectionIndex) & occupied; !blockers.empty()) {
                targets ^= blocker_and_beyond<Board>{}(find_first<Direction>(blockers), DirectionIndex);
        }
}

template<class Board>
auto classical(int const sq, set_t<Board> const occupied)
{
        auto targets = king_moves_empty_diagonal<Board>{}(sq);
        clear_blocker_and_beyond<Board, 0>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 1>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 2>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 3>(targets, sq, occupied);
        return targets;
}

}       // namespace ray
}       // namespace dctl::core
