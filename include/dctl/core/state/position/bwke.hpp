#pragma once

//          Copyright Rein Halbersma 2010-2019.
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
#include <type_traits>                          // enable_if_t

namespace dctl::core {
namespace bwke {

template<class Board>
class position
{
public:
        using board_type = Board;
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;

private:
        std::array<set_type, xstd::to_underlying(color::size)> m_color;
        set_type m_kings;
        set_type m_empty;
public:
        position() = default;

        constexpr position(set_type black_pawns, set_type white_pawns, set_type black_kings, set_type white_kings) // Throws: Nothing.
        :
                m_color{{black_pawns | black_kings, white_pawns | white_kings}},
                m_kings{black_kings | white_kings},
                m_empty{board_type::squares ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type black_pawns, set_type white_pawns) // Throws: Nothing.
        :
                m_color{{black_pawns, white_pawns}},
                m_kings{},
                m_empty{board_type::squares ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class Action>
        constexpr auto make(color c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        set_pieces(!c) ^= a.captured_pieces();
                        m_kings -= a.captured_pieces();
                }

                set_pieces(c).erase(a.from());
                set_pieces(c).insert(a.dest());
                if (a.with() == piece::king) {
                        m_kings.erase(a.from());
                        m_kings.insert(a.dest());
                } else if (a.into() == piece::king) {
                        m_kings.insert(a.dest());
                }

                m_empty = board_type::squares ^ (pieces(black_c) | pieces(white_c));
        }

        constexpr auto pieces(color c) const noexcept
        {
                return m_color[xstd::to_underlying(c)];
        }

        template<class PieceT, std::enable_if_t<
                is_piece<PieceT>
        >...>
        constexpr auto pieces(PieceT p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawn ? pieces(pawn_c) : pieces(king_c);
                } else {
                        if constexpr (p == pawn_c) { return m_kings ^ pieces(occup_c); }
                        if constexpr (p == king_c) { return m_kings;                   }
                }
        }

        template<class PieceT, std::enable_if_t<
                is_piece<PieceT>
        >...>
        constexpr auto pieces(color c, PieceT p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawn ? pieces(c, pawn_c) : pieces(c, king_c);
                } else {
                        if constexpr (p == pawn_c) { return pieces(c) - m_kings; }
                        if constexpr (p == king_c) { return pieces(c) & m_kings; }
                }
        }

        constexpr auto pieces(board_) const noexcept
        {
                return board_type::squares;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return board_type::squares ^ m_empty;
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).ssize();
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_color, m_kings);
        }

private:
        constexpr auto& set_pieces(color c) noexcept
        {
                return m_color[xstd::to_underlying(c)];
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

}       // namespace bwke
}       // namespace dctl::core
