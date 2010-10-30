#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename, size_t>
class BIT2SQUARE;

template<typename, size_t>
class SQUARE2BIT;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "GhostMetaTemplates.hpp"
