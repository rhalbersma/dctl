#pragma once

#ifdef _MSC_VER
	typedef unsigned __int8  uint8_t;
	typedef          __int16  int16_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;
#else
	#include <cstddef>
	#include <stdint.h>
#endif

typedef uint8_t PlyCount;
typedef uint64_t BitBoard;
typedef uint64_t HashIndex;
typedef uint64_t NodeCount;

static const size_t BITBOARD_CAPACITY = 8 * sizeof(BitBoard);

template<typename> struct is_IntegerType        { enum { VALUE = false }; };
template<> struct is_IntegerType<uint8_t>       { enum { VALUE = true  }; };
template<> struct is_IntegerType< int16_t>      { enum { VALUE = true  }; };
template<> struct is_IntegerType<uint16_t>      { enum { VALUE = true  }; };
template<> struct is_IntegerType<uint32_t>      { enum { VALUE = true  }; };
template<> struct is_IntegerType<uint64_t>      { enum { VALUE = true  }; };
