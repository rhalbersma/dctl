#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp>      // black, white
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert

namespace dctl::core {

template<class Token>
auto read_color(char const c)
{
        switch (c) {
        case Token::black: return color::black;
        case Token::white: return color::white;
        default: assert(false); return color::black;
        }
}

template<class Token>
char write_color(color const c)
{
        return Token::color[xstd::to_underlying_type(c)];
}

}       // namespace dctl::core
