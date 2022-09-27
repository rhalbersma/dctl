#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/state/color.hpp>            // color, black_c, white_c
#include <dctl/core/state/piece.hpp>            // piece,  pawn_c,  king_c, board_, empty_, occup_
#include <dctl/core/state/position/legal.hpp>   // is_legal
#include <dctl/util/type_traits.hpp>            // set_t
#include <concepts>                             // same_as
#include <tuple>                                // tie

namespace dctl::core {
namespace wpo {

template<class Board>
class position
{
public:
        using board_type = Board;
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;

private:
        set_type m_white;
        set_type m_pawns;
        set_type m_occup;
public:
        position() = default;
        bool operator==(position const&) const = default;

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

        template<class ColorT, class Action>
                requires is_color<ColorT>
        constexpr auto make(ColorT c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        m_white -= a.captured_pieces();
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }

                m_occup.pop(a.from());
                m_occup.add(a.dest());
                if constexpr (std::same_as<ColorT, color>) {
                        if (c == color::white) {
                                m_white.pop(a.from());
                                m_white.add(a.dest());
                        }
                } else if constexpr (std::same_as<ColorT, white_>) {
                        m_white.pop(a.from());
                        m_white.add(a.dest());
                }
                if (a.with() == piece::pawn) {
                        m_pawns.pop(a.from());
                }
                if (a.into() == piece::pawn) {
                        m_pawns.add(a.dest());
                }
        }

        template<class ColorT>
                requires is_color<ColorT>
        constexpr auto pieces(ColorT c) const noexcept
        {
                if constexpr (std::same_as<ColorT, color>) {
                        return c == color::black ? pieces(black_c) : pieces(white_c);
                } else {
                        if constexpr (c == black_c) { return m_white ^ m_occup; }
                        if constexpr (c == white_c) { return m_white;           }
                }
        }

        template<class PieceT>
                requires is_piece<PieceT>
        constexpr auto pieces(PieceT p) const noexcept
        {
                if constexpr (std::same_as<PieceT, piece>) {
                        return p == piece::pawn ? pieces(pawn_c) : pieces(king_c);
                } else {
                        if constexpr (p == pawn_c) { return m_pawns;           }
                        if constexpr (p == king_c) { return m_pawns ^ m_occup; }
                }
        }

        template<class ColorT, class PieceT>
                requires is_color<ColorT> && is_piece<PieceT>
        constexpr auto pieces(ColorT c, PieceT p) const noexcept
        {
                if constexpr (std::same_as<ColorT, color> && std::same_as<PieceT, piece>) {
                        return c == color::black ?
                                (p == piece::pawn ? pieces(black_c, pawn_c) : pieces(black_c, king_c)) :
                                (p == piece::pawn ? pieces(white_c, pawn_c) : pieces(white_c, king_c))
                        ;
                } else if constexpr (std::same_as<ColorT, color>) {
                        return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
                } else if constexpr (std::same_as<PieceT, piece>) {
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
                return mask_type::squares;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return mask_type::squares ^ m_occup;
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return m_occup;
        }

        template<class... Args>
                requires (sizeof...(Args) <= 2)
        constexpr auto num_pieces(Args&&... args) const noexcept
        {
                return pieces(std::forward<Args>(args)...).ssize();
        }
};

}       // namespace wpo
}       // namespace dctl::core
