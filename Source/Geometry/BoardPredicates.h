#pragma once

namespace geometry {

template<typename, int>
class IS_VALID;

template<typename, bool, int>
class IS_INITIAL;

template<typename, bool, int, int>
struct IS_ROW_MASK;

template<typename, bool, int, int>
struct IS_COL_MASK;

template<typename, int, int>
class IS_MAN_JUMP_GROUP;

template<typename, int, int>
class IS_JUMPABLE;

}       // namespace geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BoardPredicates.hpp"
