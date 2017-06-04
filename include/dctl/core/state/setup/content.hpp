#pragma once
#include <dctl/core/state/color_piece.hpp>    // black, white
#include <dctl/util/type_traits.hpp>    // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <sstream>                      // stringstream

namespace dctl {
namespace core {
namespace setup {

template<class Token, class State>
auto content(State const& s, int const n)
{
        assert(n < set_t<State>::max_size());

        std::stringstream sstr;
        if (s.pieces(black_c).test(n)) {
                if (s.pieces(kings_c).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(color::black)];   // black king
                else
                        sstr << Token::lower[xstd::to_underlying_type(color::black)];   // black pawn
        } else if (s.pieces(white_c).test(n)) {
                if (s.pieces(kings_c).test(n))
                        sstr << Token::upper[xstd::to_underlying_type(color::white)];   // white king
                else
                        sstr << Token::lower[xstd::to_underlying_type(color::white)];   // white pawn
        } else
                sstr << Token::empty;                                                   // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace core
}       // namespace dctl
