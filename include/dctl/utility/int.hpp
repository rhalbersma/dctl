#pragma once
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
        std::integral_constant<int, 8 * sizeof(T)>
{};

template<class T, class U>
struct num_blocks
:
        std::integral_constant<int, sizeof(T) / sizeof(U)>
{};

// primary template
template<class> struct log2_sizeof;

// specializations for integral types
template<> struct log2_sizeof<uint8_t> : std::integral_constant<int, 3> {};
template<> struct log2_sizeof<uint16_t>: std::integral_constant<int, 4> {};
template<> struct log2_sizeof<uint32_t>: std::integral_constant<int, 5> {};
template<> struct log2_sizeof<uint64_t>: std::integral_constant<int, 6> {};

// primary template
template<int> struct exp2_typeof;

// specializations for integral types
template<> struct exp2_typeof<3>        { using type = uint8_t; };
template<> struct exp2_typeof<4>        { using type = uint16_t; };
template<> struct exp2_typeof<5>        { using type = uint32_t; };
template<> struct exp2_typeof<6>        { using type = uint64_t; };

}       // namespace dctl
