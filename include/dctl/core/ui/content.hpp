#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>    // black_c, white_c
#include <dctl/core/state/piece.hpp>    // king_c
#include <utility>                      // to_underlying

namespace dctl::core {

template<class Token, class Position>
constexpr auto content(Position const& p, int const n) noexcept
{
        if (p.pieces(black_c).contains(n)) {
                if (p.pieces(king_c).contains(n)) {
                        return Token::kings[std::to_underlying(black_c())];     // black king
                } else {
                        return Token::pawns[std::to_underlying(black_c())];     // black pawn
                }
        } else if (p.pieces(white_c).contains(n)) {
                if (p.pieces(king_c).contains(n)) {
                        return Token::kings[std::to_underlying(white_c())];     // white king
                } else {
                        return Token::pawns[std::to_underlying(white_c())];     // white pawn
                }
        } else {
                return Token::empty;                                            // empty square
        }
}

}       // namespace dctl::core
