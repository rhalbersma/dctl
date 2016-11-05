#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type

namespace dctl {
namespace detail {
namespace bwke {
namespace block_adl {

template<class Board>
class BaseState
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type m_color[2];
        set_type m_kings;
        set_type m_empty;

public:
        BaseState() = default;

        BaseState(set_type const b, set_type const w, set_type const /* p */, set_type const k)
        :
                m_color{b, w},
                m_kings{k},
                m_empty{board::mask::squares_v<board_type> ^ (b | w)}
        {}

        template<class Action>
        auto& make(color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        m_kings &= ~a.captured_pieces();
                        m_empty ^= a.captured_pieces();
                }

                if (a.with() == piece::king) {
                        m_kings.reset(a.from());
                        m_kings.set(a.dest());
                } else if (a.into() == piece::king) {
                        m_kings.set(a.dest());
                }

                m_empty.set  (a.from());
                m_empty.reset(a.dest());

                return *this;
        }

        auto pieces(color const c) const noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        template<piece Type>
        auto pieces(piece_<Type> const p) const noexcept
        {
                if constexpr (p == pawn_c) { return m_kings ^ pieces(all_c); }
                if constexpr (p == king_c) { return m_kings;                 }
        }

        auto pieces(piece const p) const noexcept
        {
                return p == pawn_c ? pieces(pawn_c) : pieces(king_c);
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        template<piece Type>
        auto pieces(color const c, piece_<Type> const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces(all_) const noexcept
        {
                return board::mask::squares_v<board_type> ^ m_empty;
        }

        auto pieces(none_) const noexcept
        {
                return m_empty;
        }

private:
        auto& pieces(color const c) noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace bwke
}       // namespace detail
}       // namespace dctl
