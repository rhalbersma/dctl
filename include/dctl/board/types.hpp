#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular
#include <dctl/board/traits.hpp>        // invert, add_orthogonal_captures, remove_orthogonal_captures

namespace dctl {
namespace board {

// fit into 64-bits, even with orthogonal capture
struct micro         : rectangular< 4,  4> {};
struct mini          : rectangular< 6,  6> {};
struct checkers      : rectangular< 8,  8> {};
struct roman         :  invert_t<checkers> {};
struct spantsiretti  : rectangular<10,  8> {};
struct international : rectangular<10, 10> {};
struct frisian       : add_orthogonal_captures_t<international> {};

template<std::size_t Width, std::size_t Height, bool IsOrthogonalCaptures = true>
struct ktar          : rectangular<Width, Height, true, IsOrthogonalCaptures> {};

// fit into 64-bits if and only if without orthogonal captures
struct compact_10_12 : remove_orthogonal_captures_t<       ktar<10, 12>> {};
struct compact_12_10 : remove_orthogonal_captures_t<rectangular<12, 10>> {};

// do not fit into 64-bits, even without orthogonal capture
struct canadian      : rectangular<12, 12> {};
struct srilankan     :  invert_t<canadian> {};
struct dumm          : rectangular<14, 14> {};

}       // namespace board
}       // namespace dctl
