#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

template<int Width, int Height>
struct ktar
{
        static constexpr auto width = Width;
        static constexpr auto height = Height;
        static constexpr auto coloring = 0;
};

}       // namespace block_adl

using block_adl::ktar;

}       // namespace dctl::core
