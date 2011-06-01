#pragma once
#include <cstddef>
#include "../../Utilities/Ply.h"
#include "../../Utilities/VectorArray.h"

namespace Tree {
namespace Move {

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<size_t, MOVE_MAX> Order;
typedef VectorArray<size_t, PLY_MAX> Sequence;

}       // namespace Move
}       // namespace Tree
