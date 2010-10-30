#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename T, size_t = T::NUM_BITS - 1>
class INIT_GHOSTS;

template<typename T, bool, size_t = T::SQUARE_RANGE - 1>
class INIT_INITIAL;

template<typename T, bool, size_t, size_t = T::SQUARE_RANGE - 1>
class INIT_ROW_MASK;

template<typename T, bool, size_t, size_t = T::SQUARE_RANGE - 1>
class INIT_COL_MASK;

template<typename T, size_t, size_t = T::SQUARE_RANGE - 1>
class INIT_MAN_JUMP_GROUP;

template<typename T, size_t, size_t = T::SQUARE_RANGE - 1>
class INIT_JUMPABLE;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "BoardMetaTemplates.hpp"
