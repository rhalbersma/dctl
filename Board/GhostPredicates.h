#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, int>
class BIT_IS_GHOST;

template<typename, int>
class BIT2COORD;

template<typename, int, int>
class COORD2BIT;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "GhostPredicates.hpp"
