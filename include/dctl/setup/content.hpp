#pragma once
#include <sstream>                      // stringstream
#include <dctl/position/color.hpp>       // black, white
#include <dctl/type_traits.hpp>

namespace dctl {
namespace setup {

template<class Token, class Position>
std::string content(Position const& p, std::size_t n)
{
        assert(0 <= n && n < set_type_t<Position>::size());

        std::stringstream sstr;
        if (p.pieces(Color::black).test(n)) {
                if (p.kings().test(n))
                        sstr << Token::upper[Color::black];     // black king
                else
                        sstr << Token::lower[Color::black];     // black pawn
        } else if (p.pieces(Color::white).test(n)) {
                if (p.kings().test(n))
                        sstr << Token::upper[Color::white];     // white king
                else
                        sstr << Token::lower[Color::white];     // white pawn
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
