#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl {
namespace detail {
namespace wma {

template<class Board>
class PiecePlacement
{
public:
        using board_type = Board;
        using set_type = get_set_type<Board>;

private:
        set_type white_;
        set_type pawns_;
        set_type pieces_;

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& b, set_type const& w, set_type const& p, set_type const&)
        :
                white_{w},
                pawns_{p},
                pieces_{b | w}
        {
                assert(invariant(*this));
        }

        template<class Action>
        auto& make(Color c, Action const& a)
        {
                pieces_.reset(a.from());
                pieces_.set  (a.dest());

                if (c == Color::white) {
                        white_.reset(a.from());
                        white_.set  (a.dest());
                }

                if (a.is_with(Piece::pawn))
                        pawns_.reset(a.from());

                if (a.is_into(Piece::pawn))
                        pawns_.set(a.dest());

                if (a.is_jump()) {
                        pieces_ ^= a.captured();
                        white_ &= ~a.captured();
                        pawns_ &= ~a.captured();
                }

                assert(invariant(*this));
                return *this;
        }

        auto pieces(Color c) const noexcept
        {
                return c == Color::black ? pieces_ ^ white_ : white_;
        }

        auto pieces(Piece p) const noexcept
        {
                return p == Piece::pawn ? pawns_ : pieces_ ^ pawns_;
        }

        auto pieces(Color c, Piece p) const noexcept
        {
                return c == Color::black ?
                        (p == Piece::pawn ?  ~white_ & pawns_ : pieces_ ^ (white_ | pawns_)) :
                        (p == Piece::pawn ?   white_ & pawns_ : white_ & ~pawns_)
                ;
        }

        auto pieces() const noexcept
        {
                return pieces_;
        }

        auto not_occupied() const noexcept
        {
                return board::squares_v<Board> ^ pieces();
        }

        auto num_pieces(Color c, Piece p) const noexcept
        {
                return pieces(c, p).count();
        }
};

}       // namespace wma
}       // namespace detail
}       // namespace dctl
