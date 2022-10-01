#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

struct nano
{
        static constexpr auto width = 4;
        static constexpr auto height = 4;
};

}       // namespace block_adl

using block_adl::nano;

}       // namespace dctl::core
