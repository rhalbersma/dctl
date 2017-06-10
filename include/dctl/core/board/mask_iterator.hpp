#pragma once
#include <dctl/core/board/angle.hpp>            // angle, reverse
#include <dctl/core/board/detail/shift.hpp>            // shift_assign, shift_sign, shift_size
#include <dctl/util/type_traits.hpp>            // set_t
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/operators.hpp>                  // totally_ordered, unit_steppable
#include <iterator>                             // bidirectional_iterator_tag

namespace dctl::core {
namespace mask {

template<class Board, int Direction>
class cursor
:       boost::totally_ordered< cursor<Board, Direction>        // != >= > <=
,       boost::unit_steppable < cursor<Board, Direction>        // ++, --
> >
{
        constexpr static auto sign_incr = detail::shift_sign_v<Direction>;
        constexpr static auto sign_decr = detail::shift_sign_v<reverse(angle{Direction}).value()>;
        constexpr static auto stride    = detail::shift_size_v<Board, Direction>;
        static_assert(stride != 0, "Cursors need a non-zero stride.");

        using set_type = set_t<Board>;

        set_type m_cursor{};
public:
        cursor() = default;

        explicit cursor(set_type const c) noexcept
        :
                m_cursor{c}
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
                detail::shift_assign<sign_incr, stride>{}(m_cursor);
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        auto& operator--() noexcept
        {
                detail::shift_assign<sign_decr, stride>{}(m_cursor);
                return *this;
        }
};

template<class Board, int Direction>
using iterator = boost::counting_iterator
<
        cursor<Board, Direction>,               // Incrementable
        std::bidirectional_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction, class Set = set_t<Board>>
auto make_iterator(Set const s)
        -> iterator<Board, Direction>
{
        return { cursor<Board, Direction>{s} };
}

}       // namespace mask
}       // namespace dctl::core
