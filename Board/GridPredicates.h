#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, size_t>
class SQUARE_IS_VALID;

template<typename, size_t>
class SQUARE2COORD;

template<typename, size_t, size_t>
class COORD2SQUARE;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "GridPredicates.hpp"
