#pragma once
#include <cstdint>      // uint<X>_t with X = 8, 16, 32, 64
#include <boost/config.hpp>

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
        BOOST_STATIC_CONSTANT(int, BITS_PER_BLOCK = 8 * sizeof(B));
        BOOST_STATIC_CONSTANT(int, NUM_ENTRIES = 1 << BITS_PER_BLOCK);
        BOOST_STATIC_CONSTANT(int, INDEX[]);
        BOOST_STATIC_CONSTANT(int, COUNT[]);
};

}       // namespace bit
}       // namespace dctl

// include template definitions inside header
#include "Lookup.hpp"
