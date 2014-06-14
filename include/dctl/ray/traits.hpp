#pragma once
#include <dctl/ray/iterator.hpp>        // Iterator

namespace dctl {
namespace ray {

template<class T>
constexpr auto direction_v = Angle{};

template<class Board, int Direction>
constexpr auto direction_v<Iterator<Board, Direction>> = Angle{Direction};

}       // namespace ray
}       // namespace dctl
