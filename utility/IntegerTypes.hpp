#pragma once
#include <cstdint>                      // uint<X>_t for X = 8, 16, 32, 64
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../bit/Array.hpp"

namespace dctl {

typedef uint8_t MoveCount;
typedef uint8_t PlyCount;
typedef uint64_t NodeCount;
typedef uint64_t HashIndex;
typedef uint64_t BitBoard;
typedef uint64_t BitIndex;

template<typename T>
struct num_bits
{
        BOOST_STATIC_CONSTANT(int, value = 8 * sizeof(T));
};

template<int N, typename T>
struct num_bits< bit::Array<N, T> >
{
        BOOST_STATIC_CONSTANT(int, value = N * num_bits<T>::value);
};

template<typename> struct log2_sizeof;
template<> struct log2_sizeof<uint8_t>  { BOOST_STATIC_CONSTANT(auto, value = 3); };
template<> struct log2_sizeof<uint16_t> { BOOST_STATIC_CONSTANT(auto, value = 4); };
template<> struct log2_sizeof<uint32_t> { BOOST_STATIC_CONSTANT(auto, value = 5); };
template<> struct log2_sizeof<uint64_t> { BOOST_STATIC_CONSTANT(auto, value = 6); };

template<int> struct exp2_typeof;
template<> struct exp2_typeof<3>        { typedef uint8_t  type; };
template<> struct exp2_typeof<4>        { typedef uint16_t type; };
template<> struct exp2_typeof<5>        { typedef uint32_t type; };
template<> struct exp2_typeof<6>        { typedef uint64_t type; };

}       // namespace dctl
