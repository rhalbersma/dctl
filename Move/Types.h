#pragma once
#include "../Utilities/Ply.h"
#include "../Utilities/VectorArray.h"
#include <cstddef>

namespace Move {

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<size_t, MOVE_MAX> Order;
typedef VectorArray<size_t, PLY_MAX> Sequence;

}       // namespace move
