#pragma once
#include <dctl/board/shift.hpp>                 // shift_sign, shift_size
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/operators.hpp>                  // totally_ordered, unit_steppable, additive
#include <cstddef>                              // size_t
#include <iterator>                             // random_access_iterator_tag

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction>
class cursor
:       boost::totally_ordered< cursor<Board, Direction>        // != >= > <=
,       boost::unit_steppable < cursor<Board, Direction>        // ++, --
,       boost::additive       < cursor<Board, Direction>, int   // +, -
> > >
{
        static constexpr auto sign = board::shift_sign_v<Direction>;
        static constexpr auto N = static_cast<int>(board::shift_size_v<Board, Direction>);
        static constexpr auto stride = (sign == board::direction::right) ? -N : N;
        static_assert(stride != 0, "Cursors need a non-zero stride.");

        using square_type = std::size_t;

        int m_cursor{};
public:
        cursor() = default;

        explicit cursor(square_type const sq) noexcept
        :
                m_cursor{static_cast<int>(sq)}
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
                return static_cast<square_type>(m_cursor);
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
        auto& operator+=(std::size_t const n) noexcept
        {
                m_cursor += static_cast<int>(n) * stride;
                return *this;
        }

        // operator-(cursor, int) provided by boost::additive
        auto& operator-=(std::size_t const n) noexcept
        {
                m_cursor -= static_cast<int>(n) * stride;
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
auto make_iterator(std::size_t const sq)
        -> iterator<Board, Direction>
{
        return { cursor<Board, Direction>{sq} };
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
