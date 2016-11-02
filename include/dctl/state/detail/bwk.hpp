#pragma once
#include <dctl/color_piece.hpp>         // Color, black_type, white_type, Piece, pawn_type, king_type
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type

namespace dctl {
namespace detail {
namespace bwk {
namespace block_adl {

template<class Board>
class BaseState
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type color_[2];
        set_type kings_;

public:
        BaseState() = default;

        BaseState(set_type const b, set_type const w, set_type const k)
        :
                color_{b, w},
                kings_{k}
        {}

        BaseState(set_type const b, set_type const w, set_type const /* p */, set_type const k)
        :
                BaseState(b, w, k)
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        kings_ &= ~a.captured_pieces();
                }

                if (a.with() == Piece::king) {
                        kings_.reset(a.from());
                        kings_.set(a.dest());
                } else if (a.into() == Piece::king) {
                        kings_.set(a.dest());
                }

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return color_[xstd::to_underlying_type(c)];
        }

        auto pieces(Piece const p) const noexcept
        {
                return p == pawn_type{} ? pieces(pawn_type{}) : pieces(king_type{});
        }

        template<Piece Type>
        auto pieces(piece_constant<Type> const p) const noexcept
        {
                if constexpr (p == pawn_type{}) { return kings_ ^ pieces(any_type{}); }
                if constexpr (p == king_type{}) { return kings_;                      }
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        template<Piece Type>
        auto pieces(Color const c, piece_constant<Type> const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces(any_type) const noexcept
        {
                return pieces(black_type{}) ^ pieces(white_type{});
        }

        auto pieces(none_type) const noexcept
        {
                return board::mask::squares_v<board_type> ^ pieces(any_type{});
        }

private:
        auto& pieces(Color const c) noexcept
        {
                return color_[xstd::to_underlying_type(c)];
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace bwk
}       // namespace detail
}       // namespace dctl
