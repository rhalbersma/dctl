#pragma once
#include <dctl/board_traits.hpp>        // squares
#include <dctl/color_piece.hpp>         // color, black_, white_, piece, pawn_, king_
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <tuple>                        // tie

namespace dctl {
namespace bwk {

template<class Board>
class position
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type m_color[2];
        set_type m_kings;

public:
        position() = default;

        position(set_type const black_pawns, set_type const black_kings, set_type const white_pawns, set_type const white_kings)
        :
                m_color{black_pawns | black_kings, white_pawns | white_kings},
                m_kings{black_kings | white_kings}
        {}

        template<class Action>
        auto make(color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        m_kings &= ~a.captured_pieces();
                }

                if (a.with() == piece::king) {
                        m_kings.reset(a.from());
                        m_kings.set(a.dest());
                } else if (a.into() == piece::king) {
                        m_kings.set(a.dest());
                }
        }

        auto pieces(color const c) const noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        template<piece Type>
        auto pieces(piece_<Type> const p) const noexcept
        {
                if constexpr (p == pawns_c) { return m_kings ^ pieces(occup_c); }
                if constexpr (p == kings_c) { return m_kings;                   }
        }

        auto pieces(piece const p) const noexcept
        {
                return p == pawns_c ? pieces(pawns_c) : pieces(kings_c);
        }

        template<piece Type>
        auto pieces(color const c, piece_<Type> const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces(occup_) const noexcept
        {
                return pieces(black_c) ^ pieces(white_c);
        }

        auto pieces(empty_) const noexcept
        {
                return squares_v<board_type> ^ pieces(occup_c);
        }

        auto tied() const noexcept
        {
                return std::tie(m_color[0], m_color[1], m_kings);
        }

private:
        auto& pieces(color const c) noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
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

}       // namespace bwk
}       // namespace dctl
