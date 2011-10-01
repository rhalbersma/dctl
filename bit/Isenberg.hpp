#pragma once
#include <iomanip>
#include <iostream>
#include "../bit/Bit.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename T>
void findDeBruijn(T tried, T seq, int depth, int next) 
{
        static const int POW2N = num_bits<T>::value;
        static const int N = log2_sizeof<T>::value;
        static const int SHIFT = POW2N - N;
        static const int MOD = POW2N - 1;
        static const int FULL = POW2N >> 1;

        //std::cout << std::hex << std::setw(10) << int(tried) << " " << int(seq) << " " << int(depth) << " " << int(next) << "\n";
        if (!(tried & bit::singlet<T>(next)) && next != FULL) {
                if (!depth) {
                        //if ( ++count == num )
                                std::cout << "FOUND: " << std::hex << int(seq) << "\n";
                        //
                } else {
                        tried ^= bit::singlet<T>(next);
                        if (depth > 2 && next == (FULL - 1)) {
                                findDeBruijn<T>(tried, seq | bit::singlet<T>(depth - 1), depth - 2, 2 * next);
                        } else {
                                if (depth > 1)
                                        findDeBruijn<T>(tried, seq, depth - 1, (2 * next) & MOD);
                                findDeBruijn<T>(tried, seq | bit::singlet<T>(depth - 1), depth - 1, (2 * next + 1) & MOD);
                        }
                }
        }
}

}       // namespace dctl
