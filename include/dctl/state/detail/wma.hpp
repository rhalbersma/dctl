#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // Color, black_, white_, Piece, pawn_, king_
#include <dctl/utility/type_traits.hpp> // set_t

namespace dctl {
namespace detail {
namespace wma {
namespace block_adl {

template<class Board>
class BaseState
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type white_;
        set_type pawns_;
        set_type occupied_;

public:
        BaseState() = default;

        BaseState(set_type const w, set_type const m, set_type const a) noexcept
        :
                white_{w},
                pawns_{m},
                occupied_{a}
        {}

        BaseState(set_type const b, set_type const w, set_type const p, set_type const /* k */) noexcept
        :
                BaseState{w, p, b | w}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                occupied_.reset(a.from());
                occupied_.set  (a.dest());

                if (c == white_c) {
                        white_.reset(a.from());
                        white_.set  (a.dest());
                }

                if (a.with() == Piece::pawn)
                        pawns_.reset(a.from());

                if (a.into() == Piece::pawn)
                        pawns_.set(a.dest());

                if (a.is_jump()) {
                        white_ &= ~a.captured_pieces();
                        pawns_ &= ~a.captured_pieces();
                        occupied_   ^=  a.captured_pieces();
                }

                return *this;
        }

        template<Color Side>
        auto pieces(color_constant<Side>) const noexcept
        {
                if constexpr (Side == Color::black) { return white_ ^ occupied_; }
                if constexpr (Side == Color::white) { return white_;             }
        }

        auto pieces(Color const c) const noexcept
        {
                return c == Color::black ? pieces(black_c) : pieces(white_c);
        }

        template<Piece Type>
        auto pieces(piece_constant<Type>) const noexcept
        {
                if constexpr (Type == Piece::pawn) { return pawns_;             }
                if constexpr (Type == Piece::king) { return pawns_ ^ occupied_; }
        }

        auto pieces(Piece const p) const noexcept
        {
                return p == Piece::pawn ? pieces(pawn_c) : pieces(king_c);
        }

        template<Color Side, Piece Type>
        auto pieces(color_constant<Side>, piece_constant<Type>) const noexcept
        {
                if constexpr (Side == Color::black && Type == Piece::pawn) { return ~white_ &  pawns_;              }
                if constexpr (Side == Color::black && Type == Piece::king) { return (white_ |  pawns_) ^ occupied_; }
                if constexpr (Side == Color::white && Type == Piece::pawn) { return  white_ &  pawns_;              }
                if constexpr (Side == Color::white && Type == Piece::king) { return  white_ & ~pawns_;              }
        }

        template<Piece Type>
        auto pieces(Color const c, piece_constant<Type> const p) const noexcept
        {
                return c == Color::black ? pieces(black_c, p) : pieces(white_c, p);
        }

        template<Color Side>
        auto pieces(color_constant<Side> const c, Piece const p) const noexcept
        {
                return p == Piece::pawn ? pieces(c, pawn_c) : pieces(c, king_c);
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return pieces(
                        c == Color::black ? black_c : white_c,
                        p == Piece::pawn  ?  pawn_c :  king_c
                );
        }

        auto pieces(all_) const noexcept
        {
                return occupied_;
        }

        auto pieces(none_) const noexcept
        {
                return board::mask::squares_v<board_type> ^ occupied_;
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace wma
}       // namespace detail
}       // namespace dctl
