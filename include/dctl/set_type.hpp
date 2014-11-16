#pragma once
#include <xstd/bitset.hpp>      // bitset

namespace dctl {

template<class Board>
constexpr auto set_size = Board::bits();

template<class Board>
using set_type = xstd::bitset<set_size<Board>>;

}       // dctl
