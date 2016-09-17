#pragma once
#include <dctl/board_traits.hpp>        // invert, add_orthogonal_captures, remove_orthogonal_captures
#include <dctl/board/rectangular.hpp>   // rectangular
#include <cstddef>                      // size_t

namespace dctl {
namespace board {

using Checkers      = Rectangular< 8,  8>;
using Czech         = Rectangular< 8,  8>;
using Frisian       = add_orthogonal_captures_t<Rectangular<10, 10>>;

using Brazilian     = Rectangular< 8,  8>;
using International = Rectangular<10, 10>;
using Canadian      = Rectangular<12, 12>;
using SriLankan     = invert_t<Rectangular<12, 12>>;
using Dumm          = Rectangular<14, 14>;

using Italian       = invert_t<Rectangular< 8,  8>>;
using Pool          = Rectangular< 8,  8>;

using Russian       = Rectangular< 8,  8>;
using Shashki       = Russian;

using Spanish       = invert_t<Rectangular< 8,  8>>;
using Thai          = Rectangular< 8,  8>;

using Micro         = Rectangular< 4,  4>;
using Mini          = Rectangular< 6,  6>;

using Spantsiretti  = Rectangular<10,  8>;

template<std::size_t Width, std::size_t Height>
using Ktar          = invert_t<Rectangular<Width, Height>>;

}       // namespace board
}       // namespace dctl
