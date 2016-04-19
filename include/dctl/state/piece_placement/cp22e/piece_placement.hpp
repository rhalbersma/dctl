#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl {
namespace detail {
namespace cp22e {

template<class Board>
class PiecePlacement
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        set_type by_color_piece[2][2];
        set_type not_occupied_;

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& b, set_type const& w, set_type const& p, set_type const& k)
        :
                by_color_piece{{b & p, b & k}, {w & p, w & k}},
                not_occupied_{board::squares_v<Board> ^ (b | w)}
        {
                assert(invariant(*this));
        }

        template<class Action>
        auto& make(Color c, Action const& a)
        {
                pieces(c, a.with()).reset(a.from());
                pieces(c, a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c, Piece::pawn) &= ~a.captured();
                        pieces(!c, Piece::king) &= ~a.captured();
                        not_occupied_ ^= a.captured();
                }

                not_occupied_.set  (a.from());
                not_occupied_.reset(a.dest());

                assert(invariant(*this));
                return *this;
        }

        auto pieces(Color c) const noexcept
        {
                return pieces(c, Piece::pawn) ^ pieces(c, Piece::king);
        }

        auto pieces(Piece p) const noexcept
        {
                return pieces(Color::black, p) ^ pieces(Color::white, p);
        }

        auto pieces(Color c, Piece p) const noexcept
        {
                return by_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        auto pieces() const noexcept
        {
                return board::squares_v<Board> ^ not_occupied();
        }

        auto not_occupied() const noexcept
        {
                return not_occupied_;
        }

        auto num_pieces(Color c, Piece p) const noexcept
        {
                return pieces(c, p).count();
        }

private:
        auto& pieces(Color c, Piece p) noexcept
        {
                return by_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }
};

}       // namespace cp22e
}       // namespace detail
}       // namespace dctl
