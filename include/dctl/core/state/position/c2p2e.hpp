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
#include <xstd/utility.hpp>                     // to_underlying
#include <array>                                // array
#include <tuple>                                // tie

namespace dctl::core {
namespace c2p2e {

template<class Board>
class position
{
public:
        using board_type = Board;
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;

private:
        std::array<set_type, xstd::to_underlying(color::size)> m_color;
        std::array<set_type, xstd::to_underlying(piece::size)> m_piece;
        set_type m_empty;
public:
        position() = default;
        bool operator==(position const&) const = default;

        constexpr position(set_type black_pawns, set_type white_pawns, set_type black_kings, set_type white_kings) // Throws: Nothing.
        :
                m_color{{black_pawns | black_kings, white_pawns | white_kings}},
                m_piece{{black_pawns | white_pawns, black_kings | white_kings}},
                m_empty{mask_type::squares ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type black_pawns, set_type white_pawns) // Throws: Nothing.
        :
                m_color{{black_pawns, white_pawns}},
                m_piece{{black_pawns | white_pawns, {}}},
                m_empty{mask_type::squares ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class Action>
        constexpr auto make(color c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        set_pieces(!c) ^= a.captured_pieces();
                        set_pieces(pawn_c) -= a.captured_pieces();
                        set_pieces(king_c) -= a.captured_pieces();
                }

                set_pieces(c).pop(a.from());
                set_pieces(c).add(a.dest());
                set_pieces(a.with()).pop(a.from());
                set_pieces(a.into()).add(a.dest());

                m_empty = mask_type::squares ^ (pieces(black_c) | pieces(white_c));
        }

        constexpr auto pieces(color c) const noexcept
        {
                return m_color[xstd::to_underlying(c)];
        }

        constexpr auto pieces(piece p) const noexcept
        {
                return m_piece[xstd::to_underlying(p)];
        }

        constexpr auto pieces(color c, piece p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        constexpr auto pieces(board_) const noexcept
        {
                return mask_type::squares;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return mask_type::squares ^ m_empty;
        }

        template<class... Args>
                requires (sizeof...(Args) <= 2)
        constexpr auto num_pieces(Args&&... args) const noexcept
        {
                return pieces(std::forward<Args>(args)...).ssize();
        }

private:
        constexpr auto& set_pieces(color c) noexcept
        {
                return m_color[xstd::to_underlying(c)];
        }

        constexpr auto& set_pieces(piece p) noexcept
        {
                return m_piece[xstd::to_underlying(p)];
        }
};

}       // namespace c2p2e
}       // namespace dctl::core
