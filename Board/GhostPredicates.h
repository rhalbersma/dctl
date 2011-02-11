#pragma once

template<typename, int>
class BIT_IS_GHOST;

template<typename, int>
class BIT2COORD;

template<typename, int, int>
class COORD2BIT;

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "GhostPredicates.hpp"
