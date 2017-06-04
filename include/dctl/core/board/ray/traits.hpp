#pragma once
#include <dctl/core/board/angle.hpp>         // angle, is_positive
#include <dctl/core/board/ray/iterator.hpp>  // Iterator
#include <dctl/util/type_traits.hpp> // set_t
#include <cassert>                      // assert
#include <type_traits>                  // bool_constant

namespace dctl {
namespace core {
namespace board {
namespace ray {

template<class T>
constexpr auto direction_v = angle{};

template<class Board, int Direction>
constexpr auto direction_v<iterator<Board, Direction>> = angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(iterator<Board, Direction> it)
{
        return static_cast<unsigned>(*it) < static_cast<unsigned>(set_t<Board>::max_size());
}

}       // namespace ray
}       // namespace board
}       // namespace core
}       // namespace dctl
