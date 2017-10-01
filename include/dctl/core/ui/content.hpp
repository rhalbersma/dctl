#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp>      // black_c, white_c, kings_c
#include <xstd/type_traits.hpp>                 // to_underlying_type

namespace dctl::core {

template<class Token, class Position>
auto content(Position const& p, int const n) // Throws: Nothing.
{
        if (p.pieces(black_c).contains(n)) {
                if (p.pieces(kings_c).contains(n)) {
                        return Token::kings[xstd::to_underlying_type(black_c)]; // black king
                } else {
                        return Token::pawns[xstd::to_underlying_type(black_c)]; // black pawn
                }
        } else if (p.pieces(white_c).contains(n)) {
                if (p.pieces(kings_c).contains(n)) {
                        return Token::kings[xstd::to_underlying_type(white_c)]; // white king
                } else {
                        return Token::pawns[xstd::to_underlying_type(white_c)]; // white pawn
                }
        } else {
                return Token::empty;                                            // empty square
        }
}

}       // namespace dctl::core
