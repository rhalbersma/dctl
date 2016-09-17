#pragma once
#include <dctl/color.hpp>       // opposite
#include <dctl/mask/row.hpp>    // row

namespace dctl {
namespace mask {

template<class Board, class Color>
constexpr auto promotion_v = row<Board, opposite<Color>>{}(0);

}       // namespace mask
}       // namespace dctl
