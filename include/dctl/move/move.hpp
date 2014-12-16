#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/rules.hpp>
#include <dctl/set_type.hpp>    // set_type
#include <dctl/type_traits.hpp> // board_type_t, rules_type_t
#include <cassert>              // assert
#include <tuple>                // forward_as_tuple

namespace dctl {

template<class Rules, class Board>
class BaseMove
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using Set = set_type<Board>;

private:
        Set captured_[2]{};
        std::size_t from_{};
        std::size_t dest_{};
        Color to_move_{};
        Piece with_{Piece::pawn};
        bool is_promotion_{};

        auto invariant() const
        {
                return
                        !(from() == dest() && !is_jump()) &&
                        disjoint(captured(Piece::pawn), captured(Piece::king)) &&
                        !(with_ == Piece::king && is_promotion_)
                ;
        }

public:
        // constructors

        // king move
        constexpr BaseMove(std::size_t src, std::size_t dst, Color c)
        :
                from_{src},
                dest_{dst},
                to_move_{c},
                with_{Piece::king}
        {
                assert(invariant());
        }

        // pawn move
        constexpr BaseMove(std::size_t src, std::size_t dst, Color c, bool prom)
        :
                from_{src},
                dest_{dst},
                to_move_{c},
                is_promotion_{prom}
        {
                assert(invariant());
        }

        // any jump
        template<class Tracker>
        explicit constexpr BaseMove(Tracker const& t)
        :
                captured_{t.captured(Piece::pawn), t.captured(Piece::king)},
                from_{t.from()},
                dest_{t.dest()},
                to_move_{t.to_move()},
                with_{t.with()},
                is_promotion_{t.is_promotion()}
        {
                assert(invariant());
        }

        // observers

        constexpr auto captured(Piece p) const noexcept
        {
                return captured_[static_cast<std::size_t>(p)];
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

        constexpr auto is_promotion() const noexcept
        {
                return is_promotion_;
        }

        constexpr auto is_with_king() const noexcept
        {
                return with() == Piece::king;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured().any();
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        constexpr auto num_captured(Piece p) const noexcept
        {
                return captured(p).count();
        }

        constexpr auto num_captured() const noexcept
        {
                return captured().count();
        }

        // predicates

        friend constexpr auto
        operator==(BaseMove const& lhs, BaseMove const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.from_, lhs.dest_, lhs.captured()) ==
                        std::forward_as_tuple(rhs.from_, rhs.dest_, rhs.captured())
                ;
        }

        friend constexpr auto
        operator!=(BaseMove const& lhs, BaseMove const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        friend constexpr auto
        operator<(BaseMove const& lhs, BaseMove const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.from_, lhs.dest_, lhs.captured()) <
                        std::forward_as_tuple(rhs.from_, rhs.dest_, rhs.captured())
                ;
        }
};

template<class Rules, class Board>
class EmptyBase {};

template<class Rules, class Board>
class Move
:
        public BaseMove<Rules, Board>,
        public EmptyBase<Rules, Board>
{
public:
        using base = BaseMove<Rules, Board>;
        using base::base;
};

template<class Board>
class EmptyBase<italian::Rules, Board>
{
        set_type<Board> king_order_{};
public:
        EmptyBase() = default;
        explicit EmptyBase(set_type<Board> const& ko)
        :
                king_order_(ko)
        {}

        template<class Tracker>
        explicit EmptyBase(Tracker const& t)
        :
                king_order_(t.king_order())
        {}

        set_type<Board> king_order() const { return king_order_; }
};

template<class Board>
class Move<italian::Rules, Board>
:
        public EmptyBase<italian::Rules, Board>,
        public BaseMove<italian::Rules, Board>
{
public:
        using empty = EmptyBase<italian::Rules, Board>;
        using base = BaseMove<italian::Rules, Board>;
        using base::base;
        using Set = typename base::Set;

        // king jump
        constexpr Move(Set pieces, Set kings, int src, int dst, Color c, Set ko)
        :
                empty(ko),
                base(pieces, kings, src, dst, c, ko)
        {}

        // pawn jump
        constexpr Move(Set pieces, Set kings, int src, int dst, Color c, bool prom, Set ko)
        :
                empty(ko),
                base(pieces, kings, src, dst, c, prom, ko)
        {}

        template<class Tracker>
        explicit constexpr Move(Tracker const& t)
        :
                empty(t),
                base(t)
        {}
};

template<class T>
using Move_t = Move<rules_type_t<T>, board_type_t<T>>;

}       // namespace dctl
