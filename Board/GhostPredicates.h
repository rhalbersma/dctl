#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, size_t>
class BIT_IS_GHOST;

template<typename, size_t>
class BIT2COORD;

template<typename, size_t, size_t>
class COORD2BIT;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "GhostPredicates.hpp"
