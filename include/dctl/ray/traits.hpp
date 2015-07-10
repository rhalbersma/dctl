#pragma once
#include <dctl/angle.hpp>               // Angle, is_positive
#include <dctl/ray/iterator.hpp>        // Iterator
#include <dctl/set_type.hpp>            // set_type
#include <type_traits>                  // integral_constant

namespace dctl {
namespace ray {

template<class T>
constexpr auto direction_v = Angle{};

template<int N>
constexpr auto is_positive_v = is_positive(Angle{N});

template<int N>
using is_positive_t = std::integral_constant<bool, is_positive_v<N>>;

template<class Board, int Direction>
constexpr auto direction_v<Iterator<Board, Direction>> = Angle{Direction};

template<class Board, int Direction>
constexpr auto is_onboard(Iterator<Board, Direction> it)
{
        return *it < get_set_type<Board>::size();
}

}       // namespace ray
}       // namespace dctl
