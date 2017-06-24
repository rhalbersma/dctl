#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

template<int Width, int Height>
struct ktar
{
        constexpr static auto width = Width;
        constexpr static auto height = Height;
        constexpr static auto is_inverted = true;
};

}       // namespace block_adl

using block_adl::ktar;

}       // namespace dctl::core
