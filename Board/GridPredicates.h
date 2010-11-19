#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, int>
class SQUARE_IS_VALID;

template<typename, int>
class SQUARE2COORD;

template<typename, int, int>
class COORD2SQUARE;

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "GridPredicates.hpp"
