#pragma once
#include <dctl/player.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>         // is_king_order_precedence
#include <dctl/set_type.hpp>            // set_type
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t
#include <dctl/utility/logic.hpp>
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <cassert>                      // assert
#include <cstddef>                      // size_t
#include <tuple>                        // forward_as_tuple

namespace dctl {
namespace detail {

template<class, class, bool>
class BaseAction;

template<class Rules, class Board>
class BaseAction<Rules, Board, false>
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using   set_type = set_type<Board>;

private:
        set_type captured_[2];
        std::size_t from_;
        std::size_t dest_;
        Player to_move_;
        Piece with_;
        Piece into_;

        auto invariant() const noexcept
        {
                return
                        util::implies(from() == dest(), is_jump()) &&
                        disjoint(captured(Piece::pawn), captured(Piece::king))
                ;
        }

public:
        // pawn push
        constexpr BaseAction(std::size_t src, std::size_t dst, Player c, Piece promotion) noexcept
        :
                from_{src},
                dest_{dst},
                to_move_{c},
                with_{Piece::pawn},
                into_{promotion}
        {
                assert(invariant());
        }

        // king push
        constexpr BaseAction(std::size_t src, std::size_t dst, Player c) noexcept
        :
                from_{src},
                dest_{dst},
                to_move_{c},
                with_{Piece::king},
                into_{Piece::king}
        {
                assert(invariant());
        }

        // jump
        template<class Tracker>
        explicit constexpr BaseAction(Tracker const& t)
        :
                captured_{t.captured(Piece::pawn), t.captured(Piece::king)},
                from_{t.from()},
                dest_{t.dest()},
                to_move_{t.to_move()},
                with_{t.with()},
                into_{t.into()}
        {
                assert(invariant());
        }

        constexpr auto const& captured(Piece p) const noexcept
        {
                return captured_[xstd::to_underlying_type(p)];
        }

        constexpr auto captured() const noexcept
        {
                return captured(Piece::pawn) | captured(Piece::king);
        }

        constexpr auto from() const noexcept
        {
                return from_;
        }

        constexpr auto dest() const noexcept
        {
                return dest_;
        }

        constexpr auto to_move() const noexcept
        {
                return to_move_;
        }

        constexpr auto with() const noexcept
        {
                return with_;
        }

        constexpr auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        constexpr auto into() const noexcept
        {
                return into_;
        }

        constexpr auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(Piece::pawn) && !is_into(Piece::pawn);
        }

        constexpr auto is_jump() const noexcept
        {
                return captured().any();
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with(Piece::king) && !is_jump();
        }

        constexpr auto num_captured(Piece p) const noexcept
        {
                return captured(p).count();
        }

        constexpr auto num_captured() const noexcept
        {
                return captured(Piece::pawn).count() + captured(Piece::king).count();
        }

        auto is_equal_to(BaseAction const& other) const noexcept
        {
                return
                        std::forward_as_tuple(this->from(), this->dest(), this->captured()) ==
                        std::forward_as_tuple(other.from(), other.dest(), other.captured())
                ;
        }

        auto is_less(BaseAction const& other) const noexcept
        {
                return
                        std::forward_as_tuple(this->from(), this->dest(), this->captured()) <
                        std::forward_as_tuple(other.from(), other.dest(), other.captured())
                ;
        }
};

template<class Rules, class Board>
class BaseAction<Rules, Board, true>
:
        public BaseAction<Rules, Board, false>
{
        set_type<Board> king_order_{};

        using base = BaseAction<Rules, Board, false>;
public:
        using base::base;

        template<class Tracker>
        explicit constexpr BaseAction(Tracker const& t) noexcept
        :
                base{t},
                king_order_{t.king_order()}
        {}

        constexpr auto king_order() const noexcept
        {
                return king_order_;
        }

        auto is_equal_to(BaseAction const& other) const noexcept
        {
                return
                        std::forward_as_tuple(this->from(), this->dest(), this->captured(), this->king_order()) ==
                        std::forward_as_tuple(other.from(), other.dest(), other.captured(), other.king_order())
                ;
        }

        auto is_less(BaseAction const& other) const noexcept
        {
                return
                        std::forward_as_tuple(this->from(), this->dest(), this->captured(), this->king_order()) <
                        std::forward_as_tuple(other.from(), other.dest(), other.captured(), other.king_order())
                ;
        }
};

}       // namespace detail

template<class, class>
class Action;

template<class Rules, class Board>
auto operator==(Action<Rules, Board> const&, Action<Rules, Board> const&) noexcept;

template<class Rules, class Board>
auto operator< (Action<Rules, Board> const&, Action<Rules, Board> const&) noexcept;

template<class Rules, class Board>
class Action
:
        public detail::BaseAction<Rules, Board, precedence::is_king_order_v<Rules>>
{
        using base = detail::BaseAction<Rules, Board, precedence::is_king_order_v<Rules>>;
public:
        using base::base;

        friend auto operator== <>(Action const&, Action const&) noexcept;
        friend auto operator<  <>(Action const&, Action const&) noexcept;
};

template<class Rules, class Board>
auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return lhs.is_equal_to(rhs);
}

template<class Rules, class Board>
auto operator!=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return lhs.is_less(rhs);
}

template<class Rules, class Board>
auto operator> (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
auto operator>=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
auto operator<=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

template<class T>
using Action_t = Action<rules_type_t<T>, board_type_t<T>>;

}       // namespace dctl
