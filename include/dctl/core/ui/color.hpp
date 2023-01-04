#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>    // black, white
#include <cassert>                      // assert
#include <utility>                      // to_underlying

namespace dctl::core {

template<class Token>
constexpr auto read_color(char const c) noexcept
{
        switch (c) {
        case Token::black: return color::black;
        case Token::white: return color::white;
        default: assert(false); return color::black;
        }
}

template<class Token>
constexpr char write_color(color const c) noexcept
{
        return Token::color[std::to_underlying(c)];
}

}       // namespace dctl::core
