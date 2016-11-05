#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
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
        set_type m_white;
        set_type m_pawns;
        set_type m_occupied;

public:
        BaseState() = default;

        BaseState(set_type const w, set_type const m, set_type const a) noexcept
        :
                m_white{w},
                m_pawns{m},
                m_occupied{a}
        {}

        BaseState(set_type const b, set_type const w, set_type const p, set_type const /* k */) noexcept
        :
                BaseState{w, p, b | w}
        {}

        template<class Action>
        auto& make(color const c, Action const& a)
        {
                m_occupied.reset(a.from());
                m_occupied.set  (a.dest());

                if (c == white_c) {
                        m_white.reset(a.from());
                        m_white.set  (a.dest());
                }

                if (a.with() == piece::pawn)
                        m_pawns.reset(a.from());

                if (a.into() == piece::pawn)
                        m_pawns.set(a.dest());

                if (a.is_jump()) {
                        m_white    &= ~a.captured_pieces();
                        m_pawns    &= ~a.captured_pieces();
                        m_occupied ^=  a.captured_pieces();
                }

                return *this;
        }

        template<color Side>
        auto pieces(color_<Side>) const noexcept
        {
                if constexpr (Side == color::black) { return m_white ^ m_occupied; }
                if constexpr (Side == color::white) { return m_white;              }
        }

        auto pieces(color const c) const noexcept
        {
                return c == color::black ? pieces(black_c) : pieces(white_c);
        }

        template<piece Type>
        auto pieces(piece_<Type>) const noexcept
        {
                if constexpr (Type == piece::pawn) { return m_pawns;              }
                if constexpr (Type == piece::king) { return m_pawns ^ m_occupied; }
        }

        auto pieces(piece const p) const noexcept
        {
                return p == piece::pawn ? pieces(pawn_c) : pieces(king_c);
        }

        template<color Side, piece Type>
        auto pieces(color_<Side>, piece_<Type>) const noexcept
        {
                if constexpr (Side == color::black && Type == piece::pawn) { return ~m_white &  m_pawns;               }
                if constexpr (Side == color::black && Type == piece::king) { return (m_white |  m_pawns) ^ m_occupied; }
                if constexpr (Side == color::white && Type == piece::pawn) { return  m_white &  m_pawns;               }
                if constexpr (Side == color::white && Type == piece::king) { return  m_white & ~m_pawns;               }
        }

        template<piece Type>
        auto pieces(color const c, piece_<Type> const p) const noexcept
        {
                return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
        }

        template<color Side>
        auto pieces(color_<Side> const c, piece const p) const noexcept
        {
                return p == piece::pawn ? pieces(c, pawn_c) : pieces(c, king_c);
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(
                        c == color::black ? black_c : white_c,
                        p == piece::pawn  ?  pawn_c :  king_c
                );
        }

        auto pieces(all_) const noexcept
        {
                return m_occupied;
        }

        auto pieces(none_) const noexcept
        {
                return board::mask::squares_v<board_type> ^ m_occupied;
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace wma
}       // namespace detail
}       // namespace dctl
