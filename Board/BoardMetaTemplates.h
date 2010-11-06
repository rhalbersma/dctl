#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename T, size_t = BITBOARD_CAPACITY - 1>
class INIT_GHOSTS;

template<typename T, size_t = T::SQUARE_RANGE - 1>
class INIT_SQUARES;

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

template<typename, size_t>
class SQUARE2BIT;

template<typename, size_t>
class BIT2SQUARE;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "BoardMetaTemplates.hpp"
