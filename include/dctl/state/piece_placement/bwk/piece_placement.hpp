#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl {
namespace detail {
namespace bwk {

template<class Board>
class PiecePlacement
{
public:
        using board_type = Board;
        using set_type = get_set_type<Board>;
private:
        set_type by_color[2];
        set_type kings;

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& b, set_type const& w, set_type const&, set_type const& k)
        :
                by_color{b, w},
                kings{k}
        {
                assert(invariant(*this));
        }

        template<class Action>
        auto& make(Color c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured();
                        kings &= ~a.captured();
                }

                if (a.is_with(Piece::king)) {
                        kings.reset(a.from());
                        kings.set(a.dest());
                } else if (a.is_into(Piece::king)) {
                        kings.set(a.dest());
                }

                assert(invariant(*this));
                return *this;
        }

        auto pieces(Color c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto pieces(Piece p) const noexcept
        {
                return p == Piece::pawn ? pieces() ^ kings : kings;
        }

        auto pieces(Color c, Piece p) const noexcept
        {
                return pieces(c) & (p == Piece::pawn ? ~kings : kings);
        }

        auto pieces() const noexcept
        {
                return pieces(Color::black) ^ pieces(Color::white);
        }

        auto not_occupied() const noexcept
        {
                return board::squares_v<Board> ^ pieces();
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
};

}       // namespace bwk
}       // namespace detail
}       // namespace dctl
