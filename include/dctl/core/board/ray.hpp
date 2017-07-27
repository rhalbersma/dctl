#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/detail/shift.hpp>     // first, shift_sign, shift_size
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
        constexpr static auto sign = detail::shift_sign_v<Direction>;
        constexpr static auto N = static_cast<int>(detail::shift_size_v<Board, Direction>);
        constexpr static auto stride = (sign == detail::direction::right) ? -N : N;
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

struct inclusive_tag : std:: true_type {};
struct exclusive_tag : std::false_type {};

template<class>
struct fill;

template<>
struct fill<inclusive_tag>
{
        template<class Board, int Direction, class Set>
        auto operator()(iterator<Board, Direction> from, Set const propagator) const
        {
                auto targets = Set{};
                while (is_onboard(from) && propagator.contains(*from)) {
                        targets.insert(*from++);
                }
                return targets;
        }
};

template<>
struct fill<exclusive_tag>
{
        template<class Board, int Direction, class Set>
        auto operator()(iterator<Board, Direction> from, Set const propagator) const
        {
                return fill<inclusive_tag>{}(std::next(from), propagator);
        }
};

template<class Board, int Direction, class IncludesFrom, class Set = set_t<Board>>
class king_ray_attacks_empty
{
        constexpr static auto N = Board::bits();

        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, N>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                fill<IncludesFrom>{}(make_iterator<Board, Direction>(sq), Board::squares)
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

template<class Board, class Set = set_t<Board>>
class king_diagonal_attacks_empty
{
        constexpr static auto N = Board::bits();

        inline const static auto table = []() {
                auto result = std::array<Set, N>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                king_ray_attacks_empty<Board,  45, exclusive_tag, Set>{}(sq) |
                                king_ray_attacks_empty<Board, 135, exclusive_tag, Set>{}(sq) |
                                king_ray_attacks_empty<Board, 225, exclusive_tag, Set>{}(sq) |
                                king_ray_attacks_empty<Board, 315, exclusive_tag, Set>{}(sq)
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

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(int const sq, Set const occupied)
{
        auto targets = king_ray_attacks_empty<Board, Direction, exclusive_tag, Set>{}(sq);
        if (auto const blockers = targets & occupied; !blockers.empty()) {
                targets ^= king_ray_attacks_empty<Board, Direction, inclusive_tag, Set>{}(
                        detail::first<detail::shift_sign_v<Direction>>{}(blockers)
                );
        }
        return targets;
}

template<class Board, class Set = set_t<Board>>
auto classical_one_run(int const sq, Set const /*occupied*/)
{
        auto targets = king_diagonal_attacks_empty<Board, Set>{}(sq);
        return targets;
}

}       // namespace ray
}       // namespace dctl::core
