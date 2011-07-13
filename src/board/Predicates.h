#pragma once

namespace board {

template<typename, int>
class is_square;

template<typename, bool, int>
class is_initial;

template<typename, bool, int, int>
struct is_row_mask;

template<typename, bool, int, int>
struct is_col_mask;

template<typename, int, int>
class is_man_jump_group;

template<typename, int, int>
class is_jumpable;

template<typename, int>
class square_to_bit;

template<typename, int>
class bit_to_square;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
