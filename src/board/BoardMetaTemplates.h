#pragma once

namespace board {

template<typename T, int = T::SIZE - 1>
class INIT_SQUARES;

template<typename T, bool, int = T::SIZE - 1>
class INIT_INITIAL;

template<typename T, bool, int, int = T::SIZE - 1>
class INIT_ROW_MASK;

template<typename T, bool, int, int = T::SIZE - 1>
class INIT_COL_MASK;

template<typename T, int, int = T::SIZE - 1>
class INIT_MAN_JUMP_GROUP;

template<typename T, int, int = T::SIZE - 1>
class INIT_JUMPABLE;

template<typename, int>
class SQUARE2BIT;

template<typename, int>
class BIT2SQUARE;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BoardMetaTemplates.hpp"
