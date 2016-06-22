#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular
#include <dctl/board/traits.hpp>        // invert, add_orthogonal_captures, remove_orthogonal_captures
#include <cstddef>                      // size_t

namespace dctl {
namespace board {
namespace block_adl {

// fit into 64-bits, even with orthogonal capture
struct micro         : rectangular< 4,  4> {};
struct mini          : rectangular< 6,  6> {};

struct checkers      : rectangular< 8,  8> {};
struct roman         :  invert_t<checkers> {};

struct international : rectangular<10, 10> {};
struct frisian       : add_orthogonal_captures_t<international> {};

struct spantsiretti  : rectangular<10,  8> {};

template<std::size_t Width, std::size_t Height, bool IsOrthogonalCaptures = false>
struct ktar          : rectangular<Width, Height, true, IsOrthogonalCaptures> {};

// fit into 64-bits if and only if without orthogonal captures
struct compact_10_12 : remove_orthogonal_captures_t<       ktar<10, 12>> {};
struct compact_12_10 : remove_orthogonal_captures_t<rectangular<12, 10>> {};

// do not fit into 64-bits, even without orthogonal capture
struct canadian      : rectangular<12, 12> {};
struct srilankan     :  invert_t<canadian> {};

struct dumm          : rectangular<14, 14> {};

}       // namespace block_adl

using block_adl::micro;
using block_adl::mini;
using block_adl::checkers;
using block_adl::roman;
using block_adl::spantsiretti;
using block_adl::international;
using block_adl::frisian;
using block_adl::ktar;
using block_adl::compact_10_12;
using block_adl::compact_12_10;
using block_adl::canadian;
using block_adl::srilankan;
using block_adl::dumm;

}       // namespace board
}       // namespace dctl
