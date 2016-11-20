#pragma once
#include <dctl/color_piece.hpp>               // black, white
#include <dctl/utility/type_traits.hpp>
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <sstream>                      // stringstream

namespace dctl {
namespace setup {

template<class Token, class State>
auto content(State const& s, int const n)
{
        assert(n < set_t<State>::size());

        std::stringstream sstr;
        if (s.pieces(color::black).test(n)) {
                if (s.pieces(piece::king).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(color::black)];   // black king
                else
                        sstr << Token::lower[xstd::to_underlying_type(color::black)];   // black pawn
        } else if (s.pieces(color::white).test(n)) {
                if (s.pieces(piece::king).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(color::white)];   // white king
                else
                        sstr << Token::lower[xstd::to_underlying_type(color::white)];   // white pawn
        } else
                sstr << Token::empty;                                                   // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
