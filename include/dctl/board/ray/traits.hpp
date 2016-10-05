#pragma once
#include <dctl/board/angle.hpp>         // Angle, is_positive
#include <dctl/board/ray/iterator.hpp>  // Iterator
#include <dctl/utility/type_traits.hpp> // set_t
#include <type_traits>                  // bool_constant

namespace dctl {
namespace board {
namespace ray {

template<class T>
constexpr auto direction_v = Angle{};

template<class Board, int Direction>
constexpr auto direction_v<iterator<Board, Direction>> = Angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(iterator<Board, Direction> it)
{
        return *it < set_t<Board>::size();
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
