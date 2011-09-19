#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <iomanip>                      // std::dec, std::setfill, std::setw
#include <iostream>                     // std::cout
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {

// forward declarations
template<typename T> T get_first(T);	// least significant 1-bit
template<typename T> int index_loop(T); // index of a set 1-bit
template<typename T> int count_loop(T); // number of set 1-bits

template
<
        typename Block = uint8_t
>
class Lookup_
{
public:
        template<typename T> 
        static int index(T t)
        {
                BOOST_ASSERT(is_single(t));
                const int num_blocks = sizeof(T) / sizeof(Block);       // auto would give unsigned type!
                for (auto i = 0; i < num_blocks; ++i)               
                        if (auto b = block(t, i))
                                return (i * BITS_PER_BLOCK) + INDEX[b];
                BOOST_ASSERT(false);
                return 0;
        }
        
        template<typename T> 
        static int count(T t)
        {
                const int num_blocks = sizeof(T) / sizeof(Block);       // auto would give unsigned type!
                auto count = 0;
                for (auto i = 0; i < num_blocks; ++i)
                        count += COUNT[block(t, i)];
                return count;
        }

        static void generate_index()
        {
                std::cout << "template<typename Block>\n";
                std::cout << "const int Lookup_<Block>::INDEX[] = {\n";
                Block i(0); 
                do {
                        if (i % 16 == 0)
                                for (auto j = 0; j < 8; ++j)
                                        std::cout << " ";
                        std::cout << std::dec << std::setw(2) << std::setfill(' ') << index_loop(get_first(i));
                        if (i != NUM_ENTRIES - 1)
                                std::cout << ",";
                        if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                                std::cout << "\n";
                        else 
                                std::cout << " ";
                } while (++i);
                std::cout << "};" << "\n\n";
        }

        static void generate_count()
        {
                std::cout << "template<typename Block>\n";
                std::cout << "const int Lookup_<Block>::COUNT[] = {\n";
                Block i(0); 
                do {
                        if (i % 16 == 0)
                                for (auto j = 0; j < 8; ++j)
                                        std::cout << " ";
                        std::cout << std::dec << std::setw(2) << std::setfill(' ') << count_loop(i);
                        if (i != NUM_ENTRIES - 1)
                                std::cout << ",";
                        if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                                std::cout << "\n";
                        else 
                                std::cout << " ";
                } while (++i);
                std::cout << "};" << "\n\n";
        }

private:
        // implementation
        template<typename T> 
        static Block block(T t, int i)
        {
                BOOST_ASSERT(i >= 0 && i < (sizeof(T) / sizeof(Block)));
                return static_cast<Block>(t >> (i * BITS_PER_BLOCK));
        }

        // representation
        BOOST_STATIC_CONSTANT(int, BITS_PER_BLOCK = num_bits<Block>::value);
        BOOST_STATIC_CONSTANT(int, NUM_ENTRIES = 1 << BITS_PER_BLOCK);
        static const int INDEX[];
        static const int COUNT[];
};

// The remainder of this file has been copied from the output of the following main program:

/*
#include "../src/bit/Lookup.hpp"

int main()
{
        using namespace dctl::bit;
        Lookup::generate_index();
        Lookup::generate_count();
}
*/

template<typename Block>
const int Lookup_<Block>::INDEX[] = {
         0,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         7,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0
};

template<typename Block>
const int Lookup_<Block>::COUNT[] = {
         0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         4,  5,  5,  6,  5,  6,  6,  7,  5,  6,  6,  7,  6,  7,  7,  8
};

typedef Lookup_<> Lookup;

}       // namespace bit
}       // namespace dctl
