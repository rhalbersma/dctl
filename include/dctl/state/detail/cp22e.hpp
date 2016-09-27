#pragma once
#include <dctl/color_piece.hpp>         // Color, black_type, white_type, Piece, pawn_type, king_type
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type

namespace dctl {
namespace detail {
namespace cp22e {
namespace block_adl {

template<class Board>
class BaseState
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type color_piece_[2][2];
        set_type none_;

public:
        BaseState() = default;

        BaseState(set_type const b, set_type const w, set_type const p, set_type const k)
        :
                color_piece_{{b & p, b & k}, {w & p, w & k}},
                none_{mask::squares_v<board_type> ^ (b | w)}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                pieces(c, a.with()).reset(a.from());
                pieces(c, a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c, pawn_type{}) &= ~a.captured_pieces();
                        pieces(!c, king_type{}) &= ~a.captured_pieces();
                        none_ ^= a.captured_pieces();
                }

                none_.set  (a.from());
                none_.reset(a.dest());

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return pieces(c, pawn_type{}) ^ pieces(c, king_type{});
        }

        auto pieces(Piece const p) const noexcept
        {
                return pieces(black_type{}, p) ^ pieces(white_type{}, p);
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        auto pieces(any_type) const noexcept
        {
                return mask::squares_v<board_type> ^ none_;
        }

        auto pieces(none_type) const noexcept
        {
                return none_;
        }

private:
        auto& pieces(Color const c, Piece const p) noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace cp22e
}       // namespace detail
}       // namespace dctl
