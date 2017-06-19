#pragma once
#include <dctl/core/state/color_piece.hpp>      // black_c, white_c
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <sstream>                              // stringstream

namespace dctl::core {
namespace detail {

template<class Token, class State>
auto content(State const& s, int const n) // Throws: Nothing.
{
        std::stringstream sstr;
        if (s.pieces(black_c).test(n)) {
                if (s.pieces(kings_c).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(black_c)];        // black king
                else
                        sstr << Token::lower[xstd::to_underlying_type(black_c)];        // black pawn
        } else if (s.pieces(white_c).test(n)) {
                if (s.pieces(kings_c).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(white_c)];        // white king
                else
                        sstr << Token::lower[xstd::to_underlying_type(white_c)];        // white pawn
        } else
                sstr << Token::empty;                                                   // empty square
        return sstr.str();
}

}       // namespace detail
}       // namespace dctl::core
