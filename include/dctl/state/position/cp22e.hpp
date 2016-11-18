#pragma once
#include <dctl/board/mask/squares.hpp>  // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <tuple>                        // tie

namespace dctl {
namespace cp22e {

template<class Board>
class position
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type m_color_piece[2][2];
        set_type m_empty;

public:
        position() = default;

        position(set_type const black_pawns, set_type const black_kings, set_type const white_pawns, set_type const white_kings)
        :
                m_color_piece{{black_pawns, black_kings}, {white_pawns, white_kings}},
                m_empty{board::mask::squares_v<board_type> ^ (black_pawns | black_kings | white_pawns | white_kings)}
        {}

        template<class Action>
        auto make(color const c, Action const& a)
        {
                pieces(c, a.with()).reset(a.from());
                pieces(c, a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c, pawns_c) &= ~a.captured_pieces();
                        pieces(!c, kings_c) &= ~a.captured_pieces();
                        m_empty ^= a.captured_pieces();
                }

                m_empty.set  (a.from());
                m_empty.reset(a.dest());
        }

        auto pieces(color const c) const noexcept
        {
                return pieces(c, pawns_c) ^ pieces(c, kings_c);
        }

        auto pieces(piece const p) const noexcept
        {
                return pieces(black_c, p) ^ pieces(white_c, p);
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return m_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        auto pieces(occup_) const noexcept
        {
                return board::mask::squares_v<board_type> ^ m_empty;
        }

        auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        auto tied() const noexcept
        {
                return std::tie(m_color_piece[0][0], m_color_piece[0][1], m_color_piece[1][0], m_color_piece[1][1]);
        }

private:
        auto& pieces(color const c, piece const p) noexcept
        {
                return m_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
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

}       // namespace cp22e
}       // namespace dctl
