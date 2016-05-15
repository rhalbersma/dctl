#pragma once
#include <dctl/board/angle.hpp>         // angle
#include <dctl/utility/shift.hpp>       // shift_assign
#include <dctl/utility/type_traits.hpp> // set_t
#include <boost/operators.hpp>          // totally_ordered, unit_steppable

namespace dctl {
namespace wave {

template<class Board, int Direction>
class Cursor
:       boost::totally_ordered< Cursor<Board, Direction>        // != >= > <=
,       boost::unit_steppable < Cursor<Board, Direction>        // ++, --
> >
{
        static constexpr auto theta = angle{Direction};
        static constexpr auto S = Board::shift_size(theta);
        static constexpr auto N = is_positive(theta) ? -S : S;
        static_assert(N != 0, "Cursors need a non-zero stride.");

        using set_type = set_t<Board>;

        set_type cursor_{};
public:
        Cursor() = default;

        explicit Cursor(set_type const& c) noexcept
        :
                cursor_{c}
        {}

        template<class, int>
        friend class Cursor;

        template<int M>
        /* implicit */ Cursor(Cursor<Board, M> const& other) noexcept
        :
                cursor_{other.cursor_}
        {}

        /* implicit */ operator auto() const noexcept
        {
                return cursor_;
        }

        // operator++(int) provided by boost::unit_steppable
        auto& operator++() noexcept
        {
                util::ShiftAssign<is_positive(theta)>{}(cursor_, S);
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        auto& operator--() noexcept
        {
                util::ShiftAssign<is_negative(theta)>{}(cursor_, S);
                return *this;
        }
};

template<class Board, int Direction>
constexpr decltype(Cursor<Board, Direction>::theta) Cursor<Board, Direction>::theta;

}       // namespace wave
}       // namespace dctl
