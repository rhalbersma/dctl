#pragma once
#include <dctl/board_traits.hpp>        // squares
#include <dctl/color_piece.hpp>         // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/utility/concepts.hpp>    // is_trivial_special_members
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <tuple>                        // tie
#include <type_traits>                  // is_pod

namespace dctl {
namespace c2p2e {

template<class Board>
class position
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = position<Board>;
                static_assert(util::is_trivial_special_members_v<T>);
                static_assert(std::is_pod<T>{});
        }

        set_t<Board> m_color[2];
        set_t<Board> m_piece[2];
        set_t<Board> m_empty;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const black_kings, set_type const white_pawns, set_type const white_kings) noexcept
        :
                m_color{black_pawns | black_kings, white_pawns | white_kings},
                m_piece{black_pawns | white_pawns, black_kings | white_kings},
                m_empty{squares_v<board_type> ^ (m_color[0] | m_color[1])}
        {}

        template<class Action>
        constexpr auto make(color const c, Action const& a) // Throws: Nothing.
        {
                pieces(c)
                        .erase(a.from())
                        .insert(a.dest())
                ;
                pieces(a.with()).erase(a.from());
                pieces(a.into()).insert(a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        pieces(pawns_c) -= a.captured_pieces();
                        pieces(kings_c) -= a.captured_pieces();
                }

                m_empty = squares_v<board_type> ^ (pieces(black_c) | pieces(white_c));
        }

        constexpr auto pieces(color const c) const noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        constexpr auto pieces(piece const p) const noexcept
        {
                return m_piece[xstd::to_underlying_type(p)];
        }

        constexpr auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return squares_v<board_type> ^ m_empty;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_color[0], m_color[1], m_piece[0], m_piece[1]);
        }

private:
        constexpr auto& pieces(color const c) noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        constexpr auto& pieces(piece const p) noexcept
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