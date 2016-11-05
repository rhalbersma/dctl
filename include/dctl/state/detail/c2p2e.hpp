#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
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
        set_type m_color[2];
        set_type m_piece[2];
        set_type m_empty;

public:
        BaseState() = default;

        BaseState(set_type const b, set_type const w, set_type const p, set_type const k)
        :
                m_color{b, w},
                m_piece{p, k},
                m_empty{board::mask::squares_v<board_type> ^ (b | w)}
        {}

        template<class Action>
        auto& make(color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        pieces(pawn_c) &= ~a.captured_pieces();
                        pieces(king_c) &= ~a.captured_pieces();
                }

                m_empty = board::mask::squares_v<board_type> ^ (pieces(black_c) | pieces(white_c));

                return *this;
        }

        auto pieces(color const c) const noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        auto pieces(piece const p) const noexcept
        {
                return m_piece[xstd::to_underlying_type(p)];
        }

        auto pieces(color const c, piece const p) const noexcept
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

        auto& pieces(piece const p) noexcept
        {
                return m_piece[xstd::to_underlying_type(p)];
        }
};

}       // namespace block_adl

using block_adl::BaseState;

}       // namespace c2p2e
}       // namespace detail
}       // namespace dctl
