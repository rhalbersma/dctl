#pragma once
#include <dctl/core/board/mask.hpp>     // basic_mask

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {

template<class Board, class Set>
constexpr auto is_off_board_pieces(Set const& occupied) noexcept
{
        return !(occupied - basic_mask<Board>::squares).empty();
}

template<class Board, class Set>
constexpr auto is_promoted_pawns(Set const& black_pawns, Set const& white_pawns) noexcept
{
        using mask_type = basic_mask<Board>;
        return
                black_pawns.intersects(mask_type::promotion(black_c)) ||
                white_pawns.intersects(mask_type::promotion(white_c))
        ;
}

template<class Set>
constexpr auto is_overlapping_pieces(Set const& black_pawns, Set const& white_pawns, Set const& black_kings, Set const& white_kings) noexcept
{
        return
                (black_pawns | black_kings).intersects(white_pawns | white_kings) ||
                (black_pawns | white_pawns).intersects(black_kings | white_kings)
        ;
}

template<class Set>
constexpr auto is_overlapping_pieces(Set const& black_pawns, Set const& white_pawns) noexcept
{
        return black_pawns.intersects(white_pawns);
}

template<class Board, class Set>
constexpr auto is_legal(Set const& black_pawns, Set const& white_pawns, Set const& black_kings, Set const& white_kings) noexcept
{
        return
                !is_off_board_pieces<Board>(black_pawns | white_pawns | black_kings | white_kings) &&
                !is_promoted_pawns<Board>(black_pawns, white_pawns) &&
                !is_overlapping_pieces(black_pawns, white_pawns, black_kings, white_kings)
        ;
}

template<class Board, class Set>
constexpr auto is_legal(Set const& black_pawns, Set const& white_pawns) noexcept
{
        return
                !is_off_board_pieces<Board>(black_pawns | white_pawns) &&
                !is_promoted_pawns<Board>(black_pawns, white_pawns) &&
                !is_overlapping_pieces(black_pawns, white_pawns)
        ;
}

}       // namespace dctl::core
