#pragma once
#include "IntegerTypes.h"

namespace dctl {
namespace bit {

template
<
        typename B = uint8_t
>
class Lookup
{
public:
        template<typename T> static int index(T);
        template<typename T> static int count(T);

        static void generate_index();
        static void generate_count();

private:
        // implementation
        template<typename T> static B block(T, int);

        // representation
        static const int BITS_PER_BLOCK = 8 * sizeof(B);
        static const int NUM_ENTRIES = 1 << BITS_PER_BLOCK;
        static const int INDEX[];
        static const int COUNT[];
};

}       // namespace bit
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Lookup.hpp"
