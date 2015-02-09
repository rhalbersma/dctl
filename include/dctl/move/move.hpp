#pragma once
#include <dctl/color.hpp>
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

template<class, class, bool>
class BaseMove;

template<class Rules, class Board>
class BaseMove<Rules, Board, false>
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using Set = set_type<Board>;

private:
        Set captured_[2];
        std::size_t from_;
        std::size_t dest_;
        Color to_move_;
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
        constexpr BaseMove(std::size_t src, std::size_t dst, Color c, Piece promotion) noexcept
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
        constexpr BaseMove(std::size_t src, std::size_t dst, Color c) noexcept
        :
                from_{src},
                dest_{dst},
                to_move_{c},
                with_{Piece::king},
                into_{Piece::king}
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
class BaseMove<Rules, Board, true>
:
        public BaseMove<Rules, Board, false>
{
        set_type<Board> king_order_{};

        using base = BaseMove<Rules, Board, false>;
public:
        using base::base;

        template<class Tracker>
        explicit constexpr BaseMove(Tracker const& t) noexcept
        :
                base{t},
                king_order_{t.king_order()}
        {}

        constexpr auto king_order() const noexcept
        {
                return king_order_;
        }
};

template<class Rules, class Board>
class Move
:
        public BaseMove<Rules, Board, is_king_order_precedence_v<Rules>>
{
        using base = BaseMove<Rules, Board, is_king_order_precedence_v<Rules>>;
public:
        using base::base;
};



template<class T>
using Move_t = Move<rules_type_t<T>, board_type_t<T>>;

}       // namespace dctl
