#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl {
namespace detail {
namespace bwke {

template<class Board>
class PiecePlacement
{
public:
        using board_type = Board;
        using set_type = get_set_type<Board>;

private:
        set_type by_color[2];
        set_type kings;
        set_type not_occupied_;

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& b, set_type const& w, set_type const&, set_type const& k)
        :
                by_color{b, w},
                kings{k},
                not_occupied_{board::squares_v<Board> ^ (b | w)}
        {
                assert(invariant(*this));
        }

        template<class Action>
        auto& make(Action const& a)
        {
                pieces(a.to_move()).reset(a.from());
                pieces(a.to_move()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!a.to_move()) ^= a.captured();
                        kings &= ~a.captured();
                        not_occupied_ ^= a.captured();
                }

                if (a.is_with(Piece::king)) {
                        kings.reset(a.from());
                        kings.set(a.dest());
                } else if (a.is_into(Piece::king)) {
                        kings.set(a.dest());
                }

                not_occupied_.set  (a.from());
                not_occupied_.reset(a.dest());

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
};

}       // namespace bwke
}       // namespace detail
}       // namespace dctl
