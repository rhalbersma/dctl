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

        // operator+(self_type, int) provided by boost::additive
        auto& operator+=(int n) noexcept
        {
                cursor_ += n * N;
                return *this;
        }

        // operator-(self_type, int) provided by boost::additive
        auto& operator-=(int n) noexcept
        {
                cursor_ -= n * N;
                return *this;
        }

        // number of increments / decrements between lhs and rhs
        friend auto operator-(StridedCursor const& lhs, StridedCursor const& rhs) noexcept
        {
                return (lhs.cursor_ - rhs.cursor_) / N;
        }

        // queries

        operator int() const noexcept
        {
                return cursor_;
        }

private:
        // representation

        int cursor_{};
};

}       // namespace ray
}       // namespace dctl
