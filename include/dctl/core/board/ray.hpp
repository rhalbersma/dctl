#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle
#include <dctl/core/board/shift.hpp>            // first, shift_sign, shift_size
#include <dctl/util/meta.hpp>                   // foldr_bitor, foldr_comma, int_c, quote
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

template<bool IsLongRanged, bool IncludesFrom, bool IncludesEdge>
struct fill
{
        template<class Board, int Direction, class Set>
        auto operator()(iterator<Board, Direction> from, Set const propagator) const
        {
                assert(is_onboard(from));
                auto const is_within = [&](auto it) {
                        return is_onboard(it) && propagator.contains(*it);
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

using king_move_directions = std::tuple
<       meta::int_c< 45>
,       meta::int_c<135>
,       meta::int_c<225>
,       meta::int_c<315>
>;

using king_jump_directions = std::tuple
<       meta::int_c<  0>
,       meta::int_c< 45>
,       meta::int_c< 90>
,       meta::int_c<135>
,       meta::int_c<180>
,       meta::int_c<225>
,       meta::int_c<270>
,       meta::int_c<315>
>;

constexpr auto king_jump_index(int const direction)
{
        return direction / 45;
}

template<class Board, class Fill>
class king_targets_sq_dir
{
        template<class Direction>
        struct fill_helper
        {
                auto operator()(int const sq) const
                {
                        return Fill{}(make_iterator<Board, Direction{}>(sq), Board::squares);
                }
        };

        inline const static auto table = []() {
                auto result = std::array<std::array<set_t<Board>, std::tuple_size<king_jump_directions>::value>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::make_array<king_jump_directions, meta::quote<fill_helper>>{}(sq)
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
using king_move_targets = king_targets_sq_dir<Board, fill<is_long_ranged_king_v<Rules>, false, true>>;

template<class Rules, class Board>
using king_jump_targets = king_targets_sq_dir<Board, fill<is_long_ranged_king_v<Rules>, false, false>>;

template<class Board, class Fill>
class king_targets_dir_sq
{
        template<class Direction>
        struct fill_helper
        {
                auto operator()() const
                {
                        auto result = std::array<set_t<Board>, Board::bits()>{};
                        xstd::for_each(Board::squares, [&](auto const sq) {
                                result[static_cast<std::size_t>(sq)] = Fill{}(make_iterator<Board, Direction{}>(sq), Board::squares);
                        });
                        return result;
                }
        };

        inline const static auto table = []() {
                return meta::make_array<king_jump_directions, meta::quote<fill_helper>>{}();
        }();
public:
        auto operator()(int const sq, int const direction_index) const
        {
                return table[static_cast<std::size_t>(direction_index)][static_cast<std::size_t>(sq)];
        }
};

template<class Board>
using blocker_and_beyond = king_targets_dir_sq<Board, fill<true, true, true>>;

template<class Rules, class Board>
class king_move_targets_diag
{
        template<class Direction>
        struct king_move_targets_helper
        {
                auto operator()(int const sq) const
                {
                        constexpr auto index = Direction{} / 45;
                        return king_move_targets<Rules, Board>{}(sq, index);
                }
        };

        inline const static auto table = []() {
                auto result = std::array<set_t<Board>, Board::bits()>{};
                xstd::for_each(Board::squares, [&](auto const sq) {
                        result[static_cast<std::size_t>(sq)] =
                                meta::foldr_bitor<king_move_directions, meta::quote<king_move_targets_helper>>{}(sq);
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
        template<class Direction>
        struct clear_blocker_and_beyond
        {
                template<class Set>
                auto operator()(int const sq, Set const& occupied, Set& targets) const
                {
                        constexpr auto index = Direction{} / 45;
                        if (auto const blockers = king_move_targets<Rules, Board>{}(sq, index) & occupied; !blockers.empty()) {
                                targets ^= blocker_and_beyond<Board>{}(find_first<Direction{}>(blockers), index);
                        }
                }
        };
public:
        template<class Set>
        auto operator()(int const sq, Set const& occupied) const
        {
                if constexpr (is_long_ranged_king_v<Rules>) {
                        auto targets = king_move_targets_diag<Rules, Board>{}(sq);
                        meta::foldr_comma<king_move_directions, meta::quote<clear_blocker_and_beyond>>{}(sq, occupied, targets);
                        return targets;
                } else {
                        return king_move_targets_diag<Rules, Board>{}(sq) - occupied;
                }
        }
};

template<class Rules, class Board, int Direction, class Set>
auto king_jump_target(iterator<Board, Direction> from, Set const& occupied)
        -> std::optional<iterator<Board, Direction>>
{
        constexpr auto index = Direction / 45;
        if (auto const blockers = king_jump_targets<Rules, Board>{}(*from, index) & occupied; !blockers.empty()) {
                return make_iterator<Board, Direction>(find_first<Direction>(blockers));
        } else {
                return std::nullopt;
        }
}

}       // namespace ray
}       // namespace dctl::core
