#pragma once
#include <climits>              // CHAR_BIT
#include <cstdint>              // uint<X>_t for X = 8, 16, 32, 64
#include <type_traits>          // integral_constant

namespace dctl {

using MoveCount = uint8_t;
using PlyCount = uint8_t;
using NodeCount = uint64_t;
using HashIndex = uint64_t;
using BitBoard = uint64_t;
using BitIndex = uint64_t;

template<class T>
struct num_bits
:
        std::integral_constant<int, CHAR_BIT * sizeof(T)>
{};

template<class U, class T>
struct num_blocks
:
        std::integral_constant<int, sizeof(U) / sizeof(T)>
{};

}       // namespace dctl
