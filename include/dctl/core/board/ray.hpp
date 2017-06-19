#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/detail/shift.hpp>     // first, shift_sign, shift_size
#include <dctl/core/board/type_traits.hpp>
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

template<class Board, int Direction, class Set>
auto fill(iterator<Board, Direction> from, Set const propagator)
{
        auto targets = Set{};
        for (++from; is_onboard(from) && propagator.test(*from); ++from) {
                targets.insert(*from);
        }
        return targets;
}

template<class Board>
class king_targets
{
        constexpr static auto N = Board::bits();

        template<int Direction>
        struct init
        {
                auto operator()() const noexcept
                {
                        auto result = std::array<set_t<Board>, N>{};
                        for (auto n = 0; n < N; ++n) {
                                result[static_cast<std::size_t>(n)] =
                                        squares_v<Board>.test(n) ?
                                        fill(make_iterator<Board, Direction>(n), squares_v<Board>) :
                                        set_t<Board>{}
                                ;
                        }
                        return result;
                }
        };

        inline static auto const table = std::array<std::array<set_t<Board>, N>, 8>
        {{
                init<  0>{}(),
                init< 45>{}(),
                init< 90>{}(),
                init<135>{}(),
                init<180>{}(),
                init<225>{}(),
                init<270>{}(),
                init<315>{}()
        }};
public:
        auto operator()(int const sq, angle const alpha) const noexcept
        {
                constexpr static auto theta = 45_deg;
                constexpr static auto beta  =  0_deg;
                auto const segment = (alpha - beta) / theta;
                return table[static_cast<std::size_t>(segment)][static_cast<std::size_t>(sq)];
        }
};

template<class Board, int Direction, class Set = set_t<Board>>
auto classical(iterator<Board, Direction> from, Set const propagator)
{
        constexpr auto theta = angle{Direction};
        auto targets = king_targets<Board>{}(*from, theta);
        auto const blockers = targets & ~propagator;
        if (not blockers.empty()) {
                auto const f = detail::first<detail::shift_sign_v<Direction>>{}(blockers);
                targets ^= king_targets<Board>{}(f, theta);
                targets.erase(f);
        }
        return targets;
}

}       // namespace ray
}       // namespace dctl::core
