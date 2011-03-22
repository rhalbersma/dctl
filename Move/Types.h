#pragma once
#include "../Position/Pieces.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/Ply.h"
#include "../Utilities/VectorArray.h"

namespace Move {

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<Pieces, MOVE_MAX> Vector;
typedef VectorArray<size_t, MOVE_MAX> Order;
typedef VectorArray<size_t, PLY_MAX> Sequence;

}       // namespace move
