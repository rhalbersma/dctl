#pragma once
#include <sstream>                      // stringstream
#include <dctl/bit/bit.hpp>
#include <dctl/node/material.hpp>
#include <dctl/bit/bitboard.hpp>

namespace dctl {
namespace setup {

template<class Token>
std::string content(Material const& m, int i)
{
        auto const b = bit::singlet<BitBoard>(i);

        std::stringstream sstr;
        if (bit::is_element(b, m.pieces(Side::black))) {
                if (bit::is_element(b, m.kings()))
                        sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (bit::is_element(b, m.pieces(Side::white))) {
                if (bit::is_element(b, m.kings()))
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
