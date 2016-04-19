#pragma once
#include <dctl/action/detail/push_jump_promote.hpp>
#include <dctl/action/detail/precedence_quality.hpp>
#include <dctl/action/detail/precedence_ordering.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>                 // is_quality_precedence, is_ordering_precedence
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // optional_base
#include <cassert>                              // assert
#include <tuple>                                // forward_as_tuple
#include <type_traits>                          // enable_if

namespace dctl {

template<class Rules, class Board>
class Action
:       public detail::PushJumpPromote<Rules, Board>
,       public xstd::optional_base<is_quality_precedence_v <Rules>, detail::PrecedenceQuality <Board>>
,       public xstd::optional_base<is_ordering_precedence_v<Rules>, detail::PrecedenceOrdering<Board>>
{
        using PushJumpPromote    = detail::PushJumpPromote<Rules, Board>;
        using PrecedenceQuality  = xstd::optional_base<is_quality_precedence_v <Rules>, detail::PrecedenceQuality <Board>>;
        using PrecedenceOrdering = xstd::optional_base<is_ordering_precedence_v<Rules>, detail::PrecedenceOrdering<Board>>;
public:
        using rules_type = Rules;
        using board_type = Board;

        Action() = default;

        using PushJumpPromote::PushJumpPromote;

        template<class Builder>
        explicit constexpr Action(Builder const& b) noexcept
        :
                PushJumpPromote{b},
                PrecedenceQuality{b},
                PrecedenceOrdering{b}
        {}

        using PushJumpPromote::num_captured;

        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_v<RulesType>>* = nullptr>
        constexpr auto num_captured(Piece p) const noexcept
        {
                return PrecedenceQuality::num_captured(p);
        }
};

template<class Rules, class Board, std::enable_if_t<!is_ordering_precedence_v<Rules>>* = nullptr>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured());
}

template<class Rules, class Board, std::enable_if_t< is_ordering_precedence_v<Rules>>* = nullptr>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured(), a.piece_order());
}

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) == as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) < as_tuple(rhs);
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
