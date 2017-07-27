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
                while (is_onboard(std::next(from, LookAhead)) && propagator.contains(*from)) {
                        targets.insert(*from++);
                }
                return targets;
        }
};

template<class Board, int Direction, class ExcludesFrom, int LookAhead>
class king_ray_attacks_empty
{
        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                fill<ExcludesFrom, LookAhead>{}(make_iterator<Board, Direction>(sq), Board::squares)
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

template<class Board, int Direction>
using king_moves_empty = king_ray_attacks_empty<Board, Direction, exclusive_tag, 0>;

template<class Board, int Direction>
using blocker_and_beyond = king_ray_attacks_empty<Board, Direction, inclusive_tag, 0>;

template<class Board, int Direction>
auto classical_approach(int const sq, set_t<Board> const occupied)
{
        auto targets = king_moves_empty<Board, Direction>{}(sq);
        if (auto const blockers = targets & occupied; !blockers.empty()) {
                targets ^= blocker_and_beyond<Board, Direction>{}(find_first<Direction>(blockers));
        }
        return targets;
}

template<class Board>
class king_diagonal_moves_empty
{
        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                king_moves_empty<Board,  45>{}(sq) |
                                king_moves_empty<Board, 135>{}(sq) |
                                king_moves_empty<Board, 225>{}(sq) |
                                king_moves_empty<Board, 315>{}(sq)
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

template<class Board, int Direction>
auto clear_blocker_and_beyond(set_t<Board>& attacks, int const sq, set_t<Board> const occupied)
{
        if (auto const blockers = king_moves_empty<Board, Direction>{}(sq) & occupied; !blockers.empty()) {
                attacks ^= blocker_and_beyond<Board, Direction>{}(find_first<Direction>(blockers));
        }
}

template<class Board>
auto classical_approach_in_one_run(int const sq, set_t<Board> const occupied)
{
        auto targets = king_diagonal_moves_empty<Board>{}(sq);
        clear_blocker_and_beyond<Board,  45>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 135>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 225>(targets, sq, occupied);
        clear_blocker_and_beyond<Board, 315>(targets, sq, occupied);
        return targets;
}

}       // namespace ray
}       // namespace dctl::core
