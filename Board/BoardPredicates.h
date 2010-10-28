#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, size_t>
struct IS_INVALID_SQUARE;

template<typename, size_t>
class IS_GHOST_BIT;

template<typename, bool, size_t>
class IS_INITIAL;

template<typename, bool, size_t, size_t>
struct IS_ROW_MASK;

template<typename, bool, size_t, size_t>
struct IS_COL_MASK;

template<typename, size_t, size_t>
class IS_MAN_JUMP_GROUP;

template<typename, size_t, size_t>
class IS_JUMPABLE;

template<typename, size_t>
class BIT_ROW;

template<typename, size_t>
class BIT_COL;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "BoardPredicates.hpp"
