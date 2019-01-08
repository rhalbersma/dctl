#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>            // color, black_c, white_c
#include <dctl/core/state/piece.hpp>            // piece,  pawn_c,  king_c, board_, empty_, occup_
#include <dctl/core/state/position/legal.hpp>   // is_legal
#include <dctl/util/type_traits.hpp>            // set_t
#include <tuple>                                // tie

namespace dctl::core {
namespace wpo {

template<class Board>
class position
{
        set_t<Board> m_white;
        set_t<Board> m_pawns;
        set_t<Board> m_occup;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type black_pawns, set_type white_pawns, set_type black_kings, set_type white_kings) // Throws: Nothing.
        :
                m_white{white_pawns | white_kings},
                m_pawns{black_pawns | white_pawns},
                m_occup{m_white | m_pawns | black_kings}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type black_pawns, set_type white_pawns) // Throws: Nothing.
        :
                m_white{white_pawns},
                m_pawns{black_pawns | white_pawns},
                m_occup{m_pawns}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class ColorT, class Action, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto make(ColorT c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        m_white -= a.captured_pieces();
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }

                m_occup.erase(a.from());
                m_occup.insert(a.dest());
                if constexpr (std::is_same_v<ColorT, color>) {
                        if (c == color::white) {
                                m_white.erase(a.from());
                                m_white.insert(a.dest());
                        }
                } else if constexpr (std::is_same_v<ColorT, white_>) {
                        m_white.erase(a.from());
                        m_white.insert(a.dest());
                }
                if (a.with() == piece::pawn) {
                        m_pawns.erase(a.from());
                }
                if (a.into() == piece::pawn) {
                        m_pawns.insert(a.dest());
                }
        }

        template<class ColorT, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto pieces(ColorT c) const noexcept
        {
                if constexpr (std::is_same_v<ColorT, color>) {
                        return c == color::black ? pieces(black_c) : pieces(white_c);
                } else {
                        if constexpr (c == black_c) { return m_white ^ m_occup; }
                        if constexpr (c == white_c) { return m_white;           }
                }
        }

        template<class PieceT, std::enable_if_t<
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(PieceT p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawn ? pieces(pawn_c) : pieces(king_c);
                } else {
                        if constexpr (p == pawn_c) { return m_pawns;           }
                        if constexpr (p == king_c) { return m_pawns ^ m_occup; }
                }
        }

        template<class ColorT, class PieceT, std::enable_if_t<
                is_color_v<ColorT> && is_piece_v<PieceT>
        >...>
        constexpr auto pieces(ColorT c, PieceT p) const noexcept
        {
                if constexpr (std::is_same_v<ColorT, color> && std::is_same_v<PieceT, piece>) {
                        return c == color::black ?
                                (p == piece::pawn ? pieces(black_c, pawn_c) : pieces(black_c, king_c)) :
                                (p == piece::pawn ? pieces(white_c, pawn_c) : pieces(white_c, king_c))
                        ;
                } else if constexpr (std::is_same_v<ColorT, color>) {
                        return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
                } else if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawn ? pieces(c, pawn_c) : pieces(c, king_c);
                } else {
                        if constexpr (c == black_c && p == pawn_c) { return  m_pawns - m_white;            }
                        if constexpr (c == black_c && p == king_c) { return (m_white | m_pawns) ^ m_occup; }
                        if constexpr (c == white_c && p == pawn_c) { return  m_white & m_pawns;            }
                        if constexpr (c == white_c && p == king_c) { return  m_white - m_pawns;            }
                }
        }

        constexpr auto pieces(board_) const noexcept
        {
                return board_type::squares;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return board_type::squares ^ m_occup;
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return m_occup;
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).ssize();
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_white, m_pawns, m_occup);
        }
};

template<class Board>
constexpr auto operator==(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class Board>
constexpr auto operator!=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Board>
constexpr auto operator< (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
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

}       // namespace wpo
}       // namespace dctl::core
