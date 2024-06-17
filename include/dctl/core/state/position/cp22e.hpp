#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/state/color.hpp>            // color, black_c, white_c
#include <dctl/core/state/piece.hpp>            // piece,  pawn_c,  king_c, board_, empty_, occup_
#include <dctl/core/state/position/legal.hpp>   // is_legal
#include <dctl/util/type_traits.hpp>            // set_t
#include <array>                                // array
#include <utility>                              // forward, to_underlying

namespace dctl::core::cp22e {

template<class Board>
class position
{
public:
        using board_type = Board;
        using  mask_type = basic_mask<board_type>;
        using   set_type = set_t<mask_type>;

private:
        std::array<
                std::array<
                        set_type,
                        std::to_underlying(piece::size)
                >,
                std::to_underlying(color::size)
        > m_color_piece;
        set_type m_empty;

public:
        position() = default;
        bool operator==(position const&) const = default;

        [[nodiscard]] constexpr position(set_type black_pawns, set_type white_pawns, set_type black_kings, set_type white_kings) noexcept
        :
                m_color_piece({ {{black_pawns, black_kings}}, {{white_pawns, white_kings}} }),
                m_empty(mask_type::squares ^ (black_pawns | white_pawns | black_kings | white_kings))
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        [[nodiscard]] constexpr position(set_type black_pawns, set_type white_pawns) noexcept
        :
                m_color_piece({ {{black_pawns, {}}}, {{white_pawns, {}}} }),
                m_empty(mask_type::squares ^ (black_pawns | white_pawns))
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        [[nodiscard]] constexpr auto make(color c, auto const& a) noexcept
        {
                if (a.is_jump()) {
                        pieces(!c, pawn_c) -= a.captured_pieces();
                        pieces(!c, king_c) -= a.captured_pieces();
                        m_empty ^= a.captured_pieces();
                }

                pieces(c, a.with()).pop(a.from());
                pieces(c, a.into()).add(a.dest());
                m_empty.add(a.from());
                m_empty.pop(a.dest());
        }

        [[nodiscard]] constexpr auto pieces(color c) const noexcept
        {
                return pieces(c, pawn_c) | pieces(c, king_c);
        }

        [[nodiscard]] constexpr auto pieces(piece p) const noexcept
        {
                return pieces(black_c, p) | pieces(white_c, p);
        }

private:
        [[nodiscard]] constexpr auto& pieces(color c, piece p) noexcept
        {
                return m_color_piece[std::to_underlying(c)][std::to_underlying(p)];
        }

public:
        [[nodiscard]] constexpr auto pieces(color c, piece p) const noexcept
        {
                return m_color_piece[std::to_underlying(c)][std::to_underlying(p)];
        }

        [[nodiscard]] constexpr auto pieces(board_) const noexcept
        {
                return mask_type::squares;
        }

        [[nodiscard]] constexpr auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        [[nodiscard]] constexpr auto pieces(occup_) const noexcept
        {
                return pieces(board_c) ^ pieces(empty_c);
        }

        [[nodiscard]] constexpr auto num_pieces(auto&&... args) const noexcept
                requires (sizeof...(args) <= 2)
        {
                return static_cast<int>(pieces(std::forward<decltype(args)>(args)...).size());
        }
};

}       // namespace dctl::core::cp22e
