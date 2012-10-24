#pragma once
#include <cstdint>                      // uint<X>_t for X = 8, 16, 32, 64
#include <boost/mpl/int.hpp>            // int_

namespace dctl {

typedef uint8_t MoveCount;
typedef uint8_t PlyCount;
typedef uint64_t NodeCount;
typedef uint64_t HashIndex;
typedef uint64_t BitBoard;
typedef uint64_t BitIndex;

template<typename T>
struct num_bits
:
        boost::mpl::int_<8 * sizeof(T)>
{};

// primary template
template<typename> struct log2_sizeof;

// specializations for integral types
template<> struct log2_sizeof<uint8_t>  : boost::mpl::int_<3> {};
template<> struct log2_sizeof<uint16_t> : boost::mpl::int_<4> {};
template<> struct log2_sizeof<uint32_t> : boost::mpl::int_<5> {};
template<> struct log2_sizeof<uint64_t> : boost::mpl::int_<6> {};

// primary template
template<int> struct exp2_typeof;

// specializations for integral types
template<> struct exp2_typeof<3>        { typedef uint8_t  type; };
template<> struct exp2_typeof<4>        { typedef uint16_t type; };
template<> struct exp2_typeof<5>        { typedef uint32_t type; };
template<> struct exp2_typeof<6>        { typedef uint64_t type; };

}       // namespace dctl
