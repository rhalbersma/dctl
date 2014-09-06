#pragma once
#include <dctl/angle.hpp>               // Angle, is_positive
#include <dctl/ray/iterator.hpp>        // Iterator
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
constexpr auto is_onboard_dispatch(Iterator<Board, Direction> it, std::true_type)
{
        return 0 <= *it;
}

template<class Board, int Direction>
constexpr auto is_onboard_dispatch(Iterator<Board, Direction> it, std::false_type)
{
        return *it < Board::set_type::size();
}

template<class Board, int Direction>
constexpr auto is_onboard(Iterator<Board, Direction> it)
{
        return is_onboard_dispatch(it, is_positive_t<Direction>{});
}

}       // namespace ray
}       // namespace dctl
