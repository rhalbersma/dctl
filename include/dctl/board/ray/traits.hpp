#pragma once
#include <dctl/board/angle.hpp>         // angle, is_positive
#include <dctl/board/ray/iterator.hpp>  // Iterator
#include <dctl/utility/type_traits.hpp> // set_t
#include <type_traits>                  // bool_constant

namespace dctl {
namespace board {
namespace ray {

template<class T>
constexpr auto direction_v = angle{};

template<int N>
constexpr auto is_positive_v = is_positive(angle{N});

template<int N>
using is_positive_t = std::bool_constant<is_positive_v<N>>;

template<class Board, int Direction>
constexpr auto direction_v<iterator<Board, Direction>> = angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(iterator<Board, Direction> it)
{
        return *it < set_t<Board>::size();
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
