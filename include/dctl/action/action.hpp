#pragma once
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>         // is_quality, is_ordering
#include <dctl/set_type.hpp>            // set_type
#include <dctl/type_traits.hpp>
#include <xstd/type_traits.hpp>         // optional_base
#include <cassert>                      // assert
#include <type_traits>                  // enable_if
#include <tuple>                        // forward_as_tuple
#include <dctl/action/base_action.hpp>
#include <dctl/action/quality.hpp>
#include <dctl/action/ordering.hpp>

namespace dctl {

template<class Rules, class Board>
class Action
:       public detail::BaseAction<Rules, Board>
,       public xstd::optional_base<precedence::is_quality_v <Rules>, detail::Quality <Board>>
,       public xstd::optional_base<precedence::is_ordering_v<Rules>, detail::Ordering<Board>>
{
        using Base             = detail::BaseAction<Rules, Board>;
        using OptionalQuality  = xstd::optional_base<precedence::is_quality_v <Rules>, detail::Quality <Board>>;
        using OptionalOrdering = xstd::optional_base<precedence::is_ordering_v<Rules>, detail::Ordering<Board>>;
public:
        Action() = default;

        using Base::Base;

        template<class Tracker>
        explicit constexpr Action(Tracker const& t) noexcept
        :
                Base{t},
                OptionalQuality{t},
                OptionalOrdering{t}
        {}

        using Base::num_captured;

        template<class SFINAE = Rules, class = std::enable_if_t<precedence::is_quality_v<SFINAE>>>
        constexpr auto num_captured(Piece p) const noexcept
        {
                assert(this->num_captured_kings() <= this->num_captured());
                return p == Piece::king ? this->num_captured_kings() : this->num_captured() - this->num_captured_kings();
        }
};

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
-> std::enable_if_t<!precedence::is_ordering_v<Rules>, bool>
{
        return
                std::forward_as_tuple(lhs.from(), lhs.dest(), lhs.captured()) ==
                std::forward_as_tuple(rhs.from(), rhs.dest(), rhs.captured())
        ;
}

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
-> std::enable_if_t<precedence::is_ordering_v<Rules>, bool>
{
        return
                std::forward_as_tuple(lhs.from(), lhs.dest(), lhs.captured(), lhs.piece_order()) ==
                std::forward_as_tuple(rhs.from(), rhs.dest(), rhs.captured(), rhs.piece_order())
        ;
}

template<class Rules, class Board>
constexpr auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
-> std::enable_if_t<!precedence::is_ordering_v<Rules>, bool>
{
        return
                std::forward_as_tuple(lhs.from(), lhs.dest(), lhs.captured()) <
                std::forward_as_tuple(rhs.from(), rhs.dest(), rhs.captured())
        ;
}

template<class Rules, class Board>
constexpr auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
-> std::enable_if_t<precedence::is_ordering_v<Rules>, bool>
{
        return
                std::forward_as_tuple(lhs.from(), lhs.dest(), lhs.captured(), lhs.piece_order()) <
                std::forward_as_tuple(rhs.from(), rhs.dest(), rhs.captured(), rhs.piece_order())
        ;
}

template<class Rules, class Board>
constexpr auto operator!=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
constexpr auto operator> (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
constexpr auto operator>=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
constexpr auto operator<=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

template<class T>
using Action_t = Action<rules_type_t<T>, board_type_t<T>>;

}       // namespace dctl
