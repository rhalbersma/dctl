#pragma once
#include <cstdint>              // uint<X>_t for X = 8, 16, 32, 64

namespace dctl {

using MoveCount = uint8_t;
using PlyCount = uint8_t;
using NodeCount = uint64_t;
using HashIndex = uint64_t;
using BitBoard = uint64_t;
using BitIndex = uint64_t;

}       // namespace dctl
