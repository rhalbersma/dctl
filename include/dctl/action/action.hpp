#pragma once
#include <dctl/action/detail/push_jump_promote.hpp>
#include <dctl/action/detail/precedence_quality.hpp>
#include <dctl/action/detail/precedence_ordering.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>                 // is_quality, is_ordering
#include <dctl/board/set_type.hpp>                    // set_type
#include <xstd/type_traits.hpp>                 // optional_base
#include <cassert>                              // assert
#include <type_traits>                          // enable_if
#include <tuple>                                // forward_as_tuple

namespace dctl {

template<class Rules, class Board>
class Action
:       public detail::PushJumpPromote<Rules, Board>
,       public xstd::optional_base<precedence::is_quality_v <Rules>, detail::PrecedenceQuality <Board>>
,       public xstd::optional_base<precedence::is_ordering_v<Rules>, detail::PrecedenceOrdering<Board>>
{
        using PushJumpPromote    = detail::PushJumpPromote<Rules, Board>;
        using PrecedenceQuality  = xstd::optional_base<precedence::is_quality_v <Rules>, detail::PrecedenceQuality <Board>>;
        using PrecedenceOrdering = xstd::optional_base<precedence::is_ordering_v<Rules>, detail::PrecedenceOrdering<Board>>;
public:
        using rules_type = Rules;
        using board_type = Board;

        Action() = default;

        using PushJumpPromote::PushJumpPromote;

        template<class Tracker>
        explicit constexpr Action(Tracker const& t) noexcept
        :
                PushJumpPromote{t},
                PrecedenceQuality{t},
                PrecedenceOrdering{t}
        {}

        using PushJumpPromote::num_captured;

        template<class RulesType = rules_type, class =
                std::enable_if_t<precedence::is_quality_v<RulesType>>
        >
        constexpr auto num_captured(Piece p) const noexcept
        {
                return PrecedenceQuality::num_captured(p);
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

}       // namespace dctl
