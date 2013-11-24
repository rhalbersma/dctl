#pragma once
#include <sstream>                      // stringstream
#include <dctl/bit/bit.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace setup {

template<class Token>
std::string content(Material const& m, int i)
{
        std::stringstream sstr;
        if (m.pieces(Side::black).test(i)) {
                if (m.kings().test(i))
                        sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (m.pieces(Side::white).test(i)) {
                if (m.kings().test(i))
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
