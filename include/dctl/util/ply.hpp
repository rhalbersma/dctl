#pragma once
#include <cstdint>                      // uint8_t

namespace dctl {

using PlyCount = uint8_t;

enum : PlyCount { AVG_MOVES = 16 };
enum : PlyCount { MAX_MOVES = 128 };
enum : PlyCount { MAX_PLY = 255 };
enum : PlyCount { MAX_MATE_MOVES = 255 };

}       // namespace dctl
