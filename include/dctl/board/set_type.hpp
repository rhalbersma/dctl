#pragma once
#include <xstd/bitset.hpp>      // bitset
#include <xstd/limits.hpp>      // align_on
#include <cstddef>              // size_t

namespace dctl {

template<class Board>
using get_set_type = xstd::bitset<xstd::align_on(Board::bits(), 64)>;

}       // namespace dctl
