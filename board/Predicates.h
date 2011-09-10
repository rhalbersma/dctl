#pragma once
#include <tuple>

namespace dctl {
namespace board {

template<typename, typename, int>
class is_square;

template<typename, typename, int>
class is_initial;

template<typename, typename, int>
struct is_row_mask;

template<typename, typename, int>
struct is_col_mask;

template<typename, typename, int>
class is_jump_group;

template<typename, typename, int>
class is_jump_start;

template<typename, int>
class square_to_bit;

template<typename, int>
class bit_to_square;

}       // namespace board
}       // namespace dctl

// include template definitions inside header
#include "Predicates.hpp"
