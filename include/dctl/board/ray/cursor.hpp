#pragma once
#include <dctl/board/angle.hpp> // angle
#include <boost/operators.hpp>  // totally_ordered, unit_steppable, additive
#include <cstddef>              // size_t

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
        static constexpr auto theta = angle{Direction};
        static constexpr auto S = Board::shift_size(theta);
        static constexpr auto N = is_positive(theta) ? -S : S;
        static_assert(N != 0, "Cursors need a non-zero stride.");

        using square_type = std::size_t;

        int cursor_{};
public:
        cursor() = default;

        explicit cursor(square_type c) noexcept
        :
                cursor_{static_cast<int>(c)}
        {}

        template<class, int>
        friend class cursor;

        template<int M>
        /* implicit */ cursor(cursor<Board, M> const& other) noexcept
        :
                cursor_{other.cursor_}
        {}

        /* implicit */ operator auto() const noexcept
        {
                return static_cast<square_type>(cursor_);
        }

        // operator++(int) provided by boost::unit_steppable
        auto& operator++() noexcept
        {
                cursor_ += N;
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        auto& operator--() noexcept
        {
                cursor_ -= N;
                return *this;
        }

        // operator+(cursor, int) provided by boost::additive
        auto& operator+=(std::size_t const n) noexcept
        {
                cursor_ += static_cast<int>(n) * N;
                return *this;
        }

        // operator-(cursor, int) provided by boost::additive
        auto& operator-=(std::size_t const n) noexcept
        {
                cursor_ -= static_cast<int>(n) * N;
                return *this;
        }

        // number of increments / decrements between lhs and rhs
        friend auto operator-(cursor const& lhs, cursor const& rhs) noexcept
        {
                return (lhs.cursor_ - rhs.cursor_) / N;
        }
};

template<class Board, int Direction>
constexpr angle cursor<Board, Direction>::theta;

}       // namespace ray
}       // namespace board
}       // namespace dctl
