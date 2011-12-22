#pragma once
#include "IntegerTypes.hpp"

namespace dctl {

const static MoveCount AVG_MOVES = 16;
const static MoveCount MAX_MOVES = 128;
const static PlyCount MAX_PLY = 255;
const static int MAX_MATE_MOVES = MAX_PLY + 1;

}       // namespace dctl
