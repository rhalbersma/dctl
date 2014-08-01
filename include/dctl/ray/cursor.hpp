#pragma once
#include <dctl/angle.hpp>               // Angle
#include <boost/operators.hpp>          // totally_ordered, unit_steppable, additive

namespace dctl {
namespace ray {

template<class Board, int Direction>
class Cursor
:       boost::totally_ordered< Cursor<Board, Direction>        // != >= > <=
,       boost::unit_steppable < Cursor<Board, Direction>        // ++, --
,       boost::additive       < Cursor<Board, Direction>, int   // +, -
> > >
{
        static constexpr auto theta = Angle{Direction};
        static constexpr auto S = Board::shift_size(theta);
        static constexpr auto N = is_positive(theta) ? -S : S;
        static_assert(N != 0, "Cursors need a non-zero stride.");

        using Square = int;

public:
        // constructors

        Cursor() = default;

        explicit Cursor(Square c) noexcept
        :
                cursor_{c}
        {}

        template<class, int>
        friend class Cursor;

        template<int M>
        Cursor(Cursor<Board, M> const& other) noexcept
        :
                cursor_{other.cursor_}
        {}

        // modifiers

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

        // operator+(Cursor, int) provided by boost::additive
        auto& operator+=(int n) noexcept
        {
                cursor_ += n * N;
                return *this;
        }

        // operator-(Cursor, int) provided by boost::additive
        auto& operator-=(int n) noexcept
        {
                cursor_ -= n * N;
                return *this;
        }

        // number of increments / decrements between lhs and rhs
        friend auto operator-(Cursor const& lhs, Cursor const& rhs) noexcept
        {
                return (lhs.cursor_ - rhs.cursor_) / N;
        }

        // queries

        /* implicit */ operator Square() const noexcept
        {
                return cursor_;
        }

private:
        // representation

        Square cursor_{};
};

}       // namespace ray
}       // namespace dctl
