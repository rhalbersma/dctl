#pragma once
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>                 // is_quality_precedence, is_ordering_precedence
#include <dctl/utility/logic.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/cstdint.hpp>
#include <xstd/type_traits.hpp>                 // optional_base
#include <cassert>                              // assert
#include <tuple>                                // forward_as_tuple
#include <type_traits>                          // enable_if

namespace dctl {

template<class Rules, class Board>
class Action
{
public:
        using  board_type = Board;
        using  rules_type = Rules;
        using    set_type = set_t<Board>;
        using square_type = xstd::uint_fast_t<set_type::size()>;

private:
        set_type captured_pieces_;
        set_type captured_kings_;
        set_type piece_order_;
        square_type from_;
        square_type dest_;
        Piece with_;
        Piece into_;

        constexpr auto invariant() const noexcept
        {
                return util::implies(from() == dest(), is_jump());
        }

public:
        Action() = default;

        constexpr Action(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                captured_pieces_{},
                captured_kings_{},
                piece_order_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::pawn},
                into_{promotion ? Piece::king : Piece::pawn}
        {
                assert(invariant());
        }

        constexpr Action(std::size_t const src, std::size_t const dst) noexcept
        :
                captured_pieces_{},
                captured_kings_{},
                piece_order_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::king},
                into_{Piece::king}
        {
                assert(invariant());
        }

        auto capture_piece(std::size_t const sq, bool const is_king)
        {
                if (is_king) {
                        captured_kings_.set(sq);
                        piece_order_.set(set_type::size() - 1 - num_captured());
                }
                captured_pieces_.set(sq);
        }

        auto release_piece(std::size_t const sq, bool const is_king)
        {
                captured_pieces_.reset(sq);
                if (is_king) {
                        piece_order_.reset(set_type::size() - 1 - num_captured());
                        captured_kings_.reset(sq);
                }
        }

        auto set_from(std::size_t const src) { from_ = static_cast<square_type>(src); }
        auto set_dest(std::size_t const dst) { dest_ = static_cast<square_type>(dst); }
        auto set_with(Piece const p) { with_ = p; }
        auto set_into(Piece const p) { into_ = p; }

        constexpr auto captured() const noexcept
        {
                return captured_pieces_;
        }

        constexpr auto captured_kings() const noexcept
        {
                return captured_kings_;
        }

        constexpr auto from() const noexcept
        {
                return static_cast<std::size_t>(from_);
        }

        constexpr auto dest() const noexcept
        {
                return static_cast<std::size_t>(dest_);
        }

        constexpr auto with() const noexcept
        {
                return with_;
        }

        constexpr auto into() const noexcept
        {
                return into_;
        }

        constexpr auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        constexpr auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured().any();
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(Piece::pawn) && !is_into(Piece::pawn);
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with(Piece::king) && !is_jump();
        }

        constexpr auto num_captured() const noexcept
        {
                return captured().count();
        }

        constexpr auto num_captured_kings() const noexcept
        {
                return captured_kings().count();
        }

        constexpr auto is_with_king() const noexcept
        {
                return is_with(Piece::king);
        }

        constexpr auto piece_order() const noexcept
        {
                return piece_order_;
        }
};

template<class Rules, class Board, std::enable_if_t<!is_ordering_precedence_or_v<Rules>>* = nullptr>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured());
}

template<class Rules, class Board, std::enable_if_t< is_ordering_precedence_or_v<Rules>>* = nullptr>
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
