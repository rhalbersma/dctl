#pragma once
#include <cstddef>

namespace Geometry {

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

}       // namespace Geometry

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "BoardPredicates.hpp"
