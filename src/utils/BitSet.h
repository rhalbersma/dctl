#pragma once
#include "IntegerTypes.h"

template<typename T, typename T, size_t N>
class BitSet
{
public:
        BitSet<T, N>& flip();
        BitSet<T, N>& operator&=(const BitSet<T, N>&);
        BitSet<T, N>& operator|=(const BitSet<T, N>&);
        BitSet<T, N>& operator^=(const BitSet<T, N>&);
        BitSet<T, N>& operator<<=(size_t);
        BitSet<T, N>& operator>>=(size_t);

        bool operator==(const BitSet<T, N>&) const;
        bool operator!=(const BitSet<T, N>&) const;

private
        static const size_t BITS_PER_BLOCK = 8 * sizeof(T);
        std::array<T, N> blocks_;
};

template<typename T, size_t N> const BitSet<T, N> operator~(const BitSet<T, N>&);
template<typename T, size_t N> const BitSet<T, N> operator&(const BitSet<T, N>&, const BitSet<T, N>&);
template<typename T, size_t N> const BitSet<T, N> operator|(const BitSet<T, N>&, const BitSet<T, N>&);
template<typename T, size_t N> const BitSet<T, N> operator^(const BitSet<T, N>&, const BitSet<T, N>&);
template<typename T, size_t N> const BitSet<T, N> operator<<(const BitSet<T, N>&, size_t);
template<typename T, size_t N> const BitSet<T, N> operator>>(const BitSet<T, N>&, size_t);

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BitSet.hpp"
