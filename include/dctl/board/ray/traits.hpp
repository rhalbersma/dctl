#pragma once
#include <dctl/board/angle.hpp>         // Angle, is_positive
#include <dctl/board/ray/iterator.hpp>  // Iterator
#include <dctl/board/set_type.hpp>      // set_type
#include <type_traits>                  // bool_constant

namespace dctl {
namespace ray {

template<class T>
constexpr auto direction_v = Angle{};

template<int N>
constexpr auto is_positive_v = is_positive(Angle{N});

template<int N>
using is_positive_t = std::bool_constant<is_positive_v<N>>;

template<class Board, int Direction>
constexpr auto direction_v<Iterator<Board, Direction>> = Angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(Iterator<Board, Direction> it)
{
        return *it < get_set_type<Board>::size();
}

}       // namespace ray
}       // namespace dctl
