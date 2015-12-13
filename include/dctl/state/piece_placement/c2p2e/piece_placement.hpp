#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl {
namespace detail {
namespace c2p2e {

template<class Board>
class PiecePlacement
{
public:
        using board_type = Board;
        using set_type = get_set_type<Board>;
private:
        set_type by_color[2];
        set_type by_piece[2];
        set_type not_occupied_;

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& b, set_type const& w, set_type const& p, set_type const& k)
        :
                by_color{b, w},
                by_piece{p, k},
                not_occupied_{board::squares_v<Board> ^ (b | w)}
        {
                assert(invariant(*this));
        }

        template<class Action>
        auto& make(Color c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured();
                        pieces(Piece::pawn ) &= ~a.captured();
                        pieces(Piece::king ) &= ~a.captured();
                }

                not_occupied_ = board::squares_v<Board> ^ (pieces(Color::black) | pieces(Color::white));

                assert(invariant(*this));
                return *this;
        }

        auto pieces(Color c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto pieces(Piece p) const noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
        }

        auto pieces(Color c, Piece p) const noexcept
        {
                return pieces(c) & pieces(p);
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
        auto& pieces(Color c) noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto& pieces(Piece p) noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
        }
};

}       // namespace c2p2e
}       // namespace detail
}       // namespace dctl
