#pragma once
#include <dctl/board_traits.hpp>        // invert, add_orthogonal_captures, remove_orthogonal_captures
#include <dctl/board/rectangular.hpp>   // rectangular
#include <cstddef>                      // size_t

namespace dctl {
namespace board {

// fit into 64-bits, even with orthogonal capture
using micro         = rectangular< 4,  4>;
using mini          = rectangular< 6,  6>;

using checkers      = rectangular< 8,  8>;
using roman         =  invert_t<checkers>;

using international = rectangular<10, 10>;
using frisian       = add_orthogonal_captures_t<international>;

using spantsiretti  = rectangular<10,  8>;

template<std::size_t Width, std::size_t Height>
using ktar          = invert_t<rectangular<Width, Height>>;

// fit into 64-bits if and only if without orthogonal captures
using compact_10_12 = remove_orthogonal_captures_t<       ktar<10, 12>>;
using compact_12_10 = remove_orthogonal_captures_t<rectangular<12, 10>>;

// do not fit into 64-bits, even without orthogonal capture
using canadian      = rectangular<12, 12>;
using srilankan     =  invert_t<canadian>;

using dumm          = rectangular<14, 14>;

}       // namespace board
}       // namespace dctl
