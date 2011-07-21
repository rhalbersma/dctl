#pragma once
#include "Pieces.h"
#include "../utils/Ply.h"
#include "../utils/VectorArray.h"

namespace dtl {

typedef Pieces Move;
//class Move: public Pieces {};

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<int, MAX_PLY> Sequence;
typedef VectorArray<int, MAX_MOVES> Order;
typedef VectorArray<Move, MAX_MOVES> Stack;

}       // namespace dtl
