#pragma once
#include <boost/operators.hpp>          // totally_ordered, unit_steppable, additive
#include <dctl/angle/angle.hpp>

namespace dctl {
namespace ray {

template<class Board, int Direction>
class StridedCursor
:
        boost::totally_ordered< StridedCursor<Board, Direction>         // != >= > <=
,       boost::unit_steppable < StridedCursor<Board, Direction>         // ++, --
,       boost::additive       < StridedCursor<Board, Direction>, int    // +, -
> > >
{
        static constexpr auto N = Board::shift_size(Angle{Direction});
        static_assert(N > 0, "Cursors need a non-zero stride.");
        using self_type = StridedCursor;

public:
        // structors

        StridedCursor() noexcept = default;

        explicit StridedCursor(int c) noexcept
        :
                cursor_{c}
        {}

        template<class, int>
        friend class StridedCursor;

        template<int M>
        StridedCursor(StridedCursor<Board, M> const& other) noexcept
        :
                cursor_{other.cursor_}
        {}

        // modifiers

        // operator++(int) provided by boost::unit_steppable
        self_type& operator++() noexcept
        {
                cursor_ += N;
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        self_type& operator--() noexcept
        {
                cursor_ -= N;
                return *this;
        }

        // operator+(self_type, int) provided by boost::additive
        self_type& operator+=(int n) noexcept
        {
                cursor_ += n * N;
                return *this;
        }

        // operator-(self_type, int) provided by boost::additive
        self_type& operator-=(int n) noexcept
        {
                cursor_ -= n * N;
                return *this;
        }

        // number of increments / decrements between lhs and rhs
        friend int operator-(self_type const& lhs, self_type const& rhs) noexcept
        {
                return (lhs.cursor_ - rhs.cursor_) / N;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend bool operator==(self_type const& lhs, self_type const& rhs) noexcept
        {
                return lhs.cursor_ == rhs.cursor_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(self_type const& lhs, self_type const& rhs) noexcept
        {
                return lhs.cursor_ < rhs.cursor_;
        }

        // views

        operator int() const noexcept
        {
                return cursor_;
        }

private:
        // representation

        int cursor_ {};
};

}       // namespace ray
}       // namespace dctl
