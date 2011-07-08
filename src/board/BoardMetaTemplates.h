#pragma once

namespace board {

template<typename T, int = T::SIZE - 1>
class init_squares;

template<typename T, bool, int = T::SIZE - 1>
class init_initial;

template<typename T, bool, int, int = T::SIZE - 1>
class init_row_mask;

template<typename T, bool, int, int = T::SIZE - 1>
class init_col_mask;

template<typename T, int, int = T::SIZE - 1>
class init_man_jump_group;

template<typename T, int, int = T::SIZE - 1>
class init_jumpable;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BoardMetaTemplates.hpp"
