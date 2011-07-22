#pragma once
#include "IntegerTypes.h"

namespace dctl {

const static MoveCount MAX_MOVES = 128;
const static PlyCount MAX_PLY = 255;
const static int MAX_GRAFTED_PLY = (MAX_PLY + 1) << 4;

}       // namespace dctl
