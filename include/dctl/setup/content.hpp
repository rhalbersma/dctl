#pragma once
#include <dctl/color.hpp>               // black, white
#include <dctl/piece.hpp>
#include <dctl/utility/type_traits.hpp>
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <sstream>                      // stringstream

namespace dctl {
namespace setup {

template<class Token, class State>
auto content(State const& p, std::size_t const n)
{
        assert(n < set_t<State>::size());

        std::stringstream sstr;
        if (p.pieces(Color::black).test(n)) {
                if (p.pieces(Piece::king).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(Color::black)];   // black king
                else
                        sstr << Token::lower[xstd::to_underlying_type(Color::black)];   // black pawn
        } else if (p.pieces(Color::white).test(n)) {
                if (p.pieces(Piece::king).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(Color::white)];   // white king
                else
                        sstr << Token::lower[xstd::to_underlying_type(Color::white)];   // white pawn
        } else
                sstr << Token::empty;                                                   // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
