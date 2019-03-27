#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>    // black_c, white_c
#include <dctl/core/state/piece.hpp>    // king_c
#include <xstd/type_traits.hpp>         // to_utype

namespace dctl::core {

template<class Token, class Position>
auto content(Position const& p, int const n) // Throws: Nothing.
{
        if (p.pieces(black_c).contains(n)) {
                if (p.pieces(king_c).contains(n)) {
                        return Token::kings[xstd::to_utype(black_c)]; // black king
                } else {
                        return Token::pawns[xstd::to_utype(black_c)]; // black pawn
                }
        } else if (p.pieces(white_c).contains(n)) {
                if (p.pieces(king_c).contains(n)) {
                        return Token::kings[xstd::to_utype(white_c)]; // white king
                } else {
                        return Token::pawns[xstd::to_utype(white_c)]; // white pawn
                }
        } else {
                return Token::empty;                                            // empty square
        }
}

}       // namespace dctl::core
