#pragma once
#include <dctl/board_traits.hpp>        // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <tuple>                        // tie

namespace dctl {
namespace c2p2e {

template<class Board>
class position
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type m_color[2];
        set_type m_piece[2];
        set_type m_empty;

public:
        position() = default;

        position(set_type const black_pawns, set_type const black_kings, set_type const white_pawns, set_type const white_kings)
        :
                m_color{black_pawns | black_kings, white_pawns | white_kings},
                m_piece{black_pawns | white_pawns, black_kings | white_kings},
                m_empty{squares_v<board_type> ^ (m_color[0] | m_color[1])}
        {}

        template<class Action>
        auto make(color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        pieces(pawns_c) &= ~a.captured_pieces();
                        pieces(kings_c) &= ~a.captured_pieces();
                }

                m_empty = squares_v<board_type> ^ (pieces(black_c) | pieces(white_c));
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

        auto pieces(occup_) const noexcept
        {
                return squares_v<board_type> ^ m_empty;
        }

        auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        auto tied() const noexcept
        {
                return std::tie(m_color[0], m_color[1], m_piece[0], m_piece[1]);
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

template<class Board>
constexpr auto operator==(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class Board>
constexpr auto operator< (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
}

template<class Board>
constexpr auto operator!=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Board>
constexpr auto operator> (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Board>
constexpr auto operator>=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Board>
constexpr auto operator<=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace c2p2e
}       // namespace dctl
