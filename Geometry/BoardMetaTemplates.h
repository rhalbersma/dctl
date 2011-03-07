#pragma once
#include <cstddef>

namespace Geometry {

template<typename T, size_t = T::SIZE - 1>
class INIT_SQUARES;

template<typename T, bool, size_t = T::SIZE - 1>
class INIT_INITIAL;

template<typename T, bool, size_t, size_t = T::SIZE - 1>
class INIT_ROW_MASK;

template<typename T, bool, size_t, size_t = T::SIZE - 1>
class INIT_COL_MASK;

template<typename T, size_t, size_t = T::SIZE - 1>
class INIT_MAN_JUMP_GROUP;

template<typename T, size_t, size_t = T::SIZE - 1>
class INIT_JUMPABLE;

template<typename, int>
class SQUARE2BIT;

template<typename, int>
class BIT2SQUARE;

}       // namespace Geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BoardMetaTemplates.hpp"
