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
#include <xstd/type_traits.hpp>                 // is_integral_constant
#include <utility>                              // forward

namespace dctl::core::wpo {

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

        [[nodiscard]] constexpr position(set_type black_pawns, set_type white_pawns, set_type black_kings, set_type white_kings) noexcept
        :
                m_white(white_pawns | white_kings),
                m_pawns(black_pawns | white_pawns),
                m_occup(m_white | m_pawns | black_kings)
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        [[nodiscard]] constexpr position(set_type black_pawns, set_type white_pawns) noexcept
        :
                m_white(white_pawns),
                m_pawns(black_pawns | white_pawns),
                m_occup(m_pawns)
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        [[nodiscard]] constexpr auto make(color c, auto const& a) noexcept
        {
                if (a.is_jump()) {
                        if (c == color::black) {
                                m_white -= a.captured_pieces();
                        }
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }

                if (c == color::white) {
                        m_white.pop(a.from());
                        m_white.add(a.dest());
                }
                if (a.with() == piece::pawn) {
                        m_pawns.pop(a.from());
                }
                if (a.into() == piece::pawn) {
                        m_pawns.add(a.dest());
                }
                m_occup.pop(a.from());
                m_occup.add(a.dest());
        }

        [[nodiscard]] constexpr auto make(auto c, auto const& a) noexcept
                requires xstd::is_integral_constant_v<decltype(c), color>
        {
                if (a.is_jump()) {
                        if constexpr (c == black_c) {
                                m_white -= a.captured_pieces();
                        }
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }

                if constexpr (c == white_c) {
                        m_white.pop(a.from());
                        m_white.add(a.dest());
                }
                if (a.with() == piece::pawn) {
                        m_pawns.pop(a.from());
                }
                if (a.into() == piece::pawn) {
                        m_pawns.add(a.dest());
                }
                m_occup.pop(a.from());
                m_occup.add(a.dest());
        }

        [[nodiscard]] constexpr auto pieces(color c) const noexcept
        {
                return c == color::black ? pieces(black_c) : pieces(white_c);
        }

        [[nodiscard]] constexpr auto pieces(auto c) const noexcept
                requires xstd::is_integral_constant_v<decltype(c), color>
        {
                if constexpr (c == black_c) { return m_white ^ m_occup; }
                if constexpr (c == white_c) { return m_white;           }
        }

        [[nodiscard]] constexpr auto pieces(piece p) const noexcept
        {
                return p == piece::pawn ? pieces(pawn_c) : pieces(king_c);
        }

        [[nodiscard]] constexpr auto pieces(auto p) const noexcept
                requires xstd::is_integral_constant_v<decltype(p), piece>
        {
                if constexpr (p == pawn_c) { return m_pawns;           }
                if constexpr (p == king_c) { return m_pawns ^ m_occup; }
        }

        [[nodiscard]] constexpr auto pieces(color c, piece p) const noexcept
        {
                return c == color::black ?
                        (p == piece::pawn ? pieces(black_c, pawn_c) : pieces(black_c, king_c)) :
                        (p == piece::pawn ? pieces(white_c, pawn_c) : pieces(white_c, king_c))
                ;
        }

        [[nodiscard]] constexpr auto pieces(auto c, piece p) const noexcept
                requires xstd::is_integral_constant_v<decltype(c), color>
        {
                return p == piece::pawn ? pieces(c, pawn_c) : pieces(c, king_c);
        }

        [[nodiscard]] constexpr auto pieces(color c, auto p) const noexcept
                requires xstd::is_integral_constant_v<decltype(p), piece>
        {
                return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
        }

        [[nodiscard]] constexpr auto pieces(auto c, auto p) const noexcept
                requires xstd::is_integral_constant_v<decltype(c), color> && xstd::is_integral_constant_v<decltype(p), piece>
        {
                if constexpr (c == black_c && p == pawn_c) { return  m_pawns - m_white;            }
                if constexpr (c == black_c && p == king_c) { return (m_white | m_pawns) ^ m_occup; }
                if constexpr (c == white_c && p == pawn_c) { return  m_white & m_pawns;            }
                if constexpr (c == white_c && p == king_c) { return  m_white - m_pawns;            }
        }

        [[nodiscard]] constexpr auto pieces(board_) const noexcept
        {
                return mask_type::squares;
        }

        [[nodiscard]] constexpr auto pieces(occup_) const noexcept
        {
                return m_occup;
        }

        [[nodiscard]] constexpr auto pieces(empty_) const noexcept
        {
                return mask_type::squares ^ m_occup;
        }

        [[nodiscard]] constexpr auto num_pieces(auto&&... args) const noexcept
                requires (sizeof...(args) <= 2)
        {
                return pieces(std::forward<decltype(args)>(args)...).ssize();
        }
};

}       // namespace dctl::core::wpo
