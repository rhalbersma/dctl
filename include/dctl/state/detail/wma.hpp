#pragma once
#include <dctl/color_piece.hpp>         // Color, black_type, white_type, Piece, pawn_type, king_type
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
        set_type any_;

public:
        BaseState() = default;

        BaseState(set_type const w, set_type const m, set_type const a) noexcept
        :
                white_{w},
                pawns_{m},
                any_{a}
        {}

        BaseState(set_type const b, set_type const w, set_type const p, set_type const /* k */) noexcept
        :
                BaseState{w, p, b | w}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                any_.reset(a.from());
                any_.set  (a.dest());

                if (c == white_type{}) {
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
                        any_   ^=  a.captured_pieces();
                }

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return c == Color::black ? pieces(black_type{}) : pieces(white_type{});
        }

        template<Color Side>
        auto pieces(color_constant<Side>) const noexcept
        {
                if constexpr (Side == Color::black) { return white_ ^ any_; }
                if constexpr (Side == Color::white) { return white_;        }
        }

        auto pieces(Piece const p) const noexcept
        {
                return p == Piece::pawn ? pieces(pawn_type{}) : pieces(king_type{});
        }

        template<Piece Type>
        auto pieces(piece_constant<Type>) const noexcept
        {
                if constexpr (Type == Piece::pawn) { return pawns_;        }
                if constexpr (Type == Piece::king) { return pawns_ ^ any_; }
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return c == Color::black ?
                        (p == Piece::pawn ?  pieces(black_type{}, pawn_type{}) : pieces(black_type{}, king_type{})) :
                        (p == Piece::pawn ?  pieces(white_type{}, pawn_type{}) : pieces(white_type{}, king_type{}))
                ;
        }

        template<Piece Type>
        auto pieces(Color const c, piece_constant<Type> const p) const noexcept
        {
                return c == Color::black ? pieces(black_type{}, p) : pieces(white_type{}, p);
        }

        template<Color Side>
        auto pieces(color_constant<Side> const c, Piece const p) const noexcept
        {
                return p == Piece::pawn ? pieces(c, pawn_type{}) : pieces(c, king_type{});
        }

        template<Color Side, Piece Type>
        auto pieces(color_constant<Side>, piece_constant<Type>) const noexcept
        {
                if constexpr (Side == Color::black && Type == Piece::pawn) { return ~white_ &  pawns_;         }
                if constexpr (Side == Color::black && Type == Piece::king) { return (white_ |  pawns_) ^ any_; }
                if constexpr (Side == Color::white && Type == Piece::pawn) { return  white_ &  pawns_;         }
                if constexpr (Side == Color::white && Type == Piece::king) { return  white_ & ~pawns_;         }
        }

        auto pieces(any_type) const noexcept
        {
                return any_;
        }

        auto pieces(none_type) const noexcept
        {
                return mask::squares_v<board_type> ^ any_;
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace wma
}       // namespace detail
}       // namespace dctl
