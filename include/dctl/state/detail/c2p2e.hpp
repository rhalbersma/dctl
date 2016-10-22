#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // Color, black_type, white_type, Piece, pawn_type, king_type
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type

namespace dctl {
namespace detail {
namespace c2p2e {
namespace block_adl {

template<class Board>
class BaseState
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type color_[2];
        set_type piece_[2];
        set_type none_;

public:
        BaseState() = default;

        BaseState(set_type const b, set_type const w, set_type const p, set_type const k)
        :
                color_{b, w},
                piece_{p, k},
                none_{board::mask::squares_v<board_type> ^ (b | w)}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        pieces(pawn_type{}) &= ~a.captured_pieces();
                        pieces(king_type{}) &= ~a.captured_pieces();
                }

                none_ = board::mask::squares_v<board_type> ^ (pieces(black_type{}) | pieces(white_type{}));

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return color_[xstd::to_underlying_type(c)];
        }

        auto pieces(Piece const p) const noexcept
        {
                return piece_[xstd::to_underlying_type(p)];
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces(any_type) const noexcept
        {
                return board::mask::squares_v<board_type> ^ none_;
        }

        auto pieces(none_type) const noexcept
        {
                return none_;
        }

private:
        auto& pieces(Color const c) noexcept
        {
                return color_[xstd::to_underlying_type(c)];
        }

        auto& pieces(Piece const p) noexcept
        {
                return piece_[xstd::to_underlying_type(p)];
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace c2p2e
}       // namespace detail
}       // namespace dctl