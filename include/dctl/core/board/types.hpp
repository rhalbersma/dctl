#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/rectangular.hpp>
#include <dctl/core/board/type_traits.hpp>

namespace dctl::core {
namespace board {

using checkers      = rectangular< 8,  8>;
using czech         = rectangular< 8,  8>;
using frisian       = add_orthogonal_captures_t<rectangular<10, 10>>;

using brazilian     = rectangular< 8,  8>;
using international = rectangular<10, 10>;
using canadian      = rectangular<12, 12>;
using srilankan     = invert_t<rectangular<12, 12>>;
using dumm          = rectangular<14, 14>;

using italian       = invert_t<rectangular< 8,  8>>;
using pool          = rectangular< 8,  8>;

using russian       = rectangular< 8,  8>;
using shashki       = russian;

using spanish       = invert_t<rectangular< 8,  8>>;
using thai          = rectangular< 8,  8>;

using micro         = rectangular< 4,  4>;
using mini          = rectangular< 6,  6>;

using spantsiretti  = rectangular<10,  8>;

template<int Width, int Height>
using ktar          = invert_t<rectangular<Width, Height>>;

}       // namespace board
}       // namespace dctl::core
