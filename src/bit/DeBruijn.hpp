#pragma once
#include <iomanip>                      // setfill, setw
#include <iostream>                     // cout
#include <typeinfo>                     // typeid
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {
namespace debruijn {

// primary template
template<typename T, typename _ = void>
struct Sequence;

template<typename T, typename _ = void>
struct Table;

// partial specialization for unsigned 8-bit integers
template<typename _>
struct Sequence<unsigned char, _>
{
        BOOST_STATIC_CONSTANT(unsigned char, value = 0x1d);
};

template<typename _>
struct Table<unsigned char, _>
{
        static int const value[];
};

template<typename _>
int const Table<unsigned char, _>::value[] = {
         0,  1,  6,  2,  7,  5,  4,  3
};

// partial specialization for unsigned 16-bit integers
template<typename _>
struct Sequence<unsigned short, _>
{
        BOOST_STATIC_CONSTANT(unsigned short, value = 0x0f65);
};

template<typename _>
struct Table<unsigned short, _>
{
        static int const value[];
};

template<typename _>
int const Table<unsigned short, _>::value[] = {
         0,  1, 11,  2, 14, 12,  8,  3,
        15, 10, 13,  7,  9,  6,  5,  4
};

// partial specialization for unsigned 32-bit integers
template<typename _>
struct Sequence<unsigned int, _>
{
        BOOST_STATIC_CONSTANT(unsigned int, value = 0x07dcd629);
};

template<typename _>
struct Table<unsigned int, _>
{
        static int const value[];
};

template<typename _>
int const Table<unsigned int, _>::value[] = {
         0,  1, 23,  2, 29, 24, 14,  3,
        30, 27, 25, 18, 20, 15, 10,  4,
        31, 22, 28, 13, 26, 17, 19,  9,
        21, 12, 16,  8, 11,  7,  6,  5
};

// partial specialization for unsigned 64-bit integers
template<typename _>
struct Sequence<unsigned __int64, _>
{
        BOOST_STATIC_CONSTANT(unsigned __int64, value = 0x03f79d71b4cb0a89);
};

template<typename _>
struct Table<unsigned __int64, _>
{
        static int const value[];
};

template<typename _>
int const Table<unsigned __int64, _>::value[] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
};

/*

        Find a B(2, N) DeBruijn-sequence by generating an Eulerian circuit
        on the B(2, N-1) DeBruijn-graph.

*/

template<int N>
struct Graph
{
        typedef typename exp2_typeof<N + 1>::type Edge;
        BOOST_STATIC_CONSTANT(int, edge_mask = (1 << (N+1)) - 1);
        BOOST_STATIC_CONSTANT(int, num_vertices = 1 << N);
        BOOST_STATIC_CONSTANT(uint64_t, all_vertices = (uint64_t(1) << num_vertices) - 1);

        static NodeCount Eulerian(int vertex = 0, bool branch = 0, int depth = edge_mask, Edge sequence = 0, Edge tour = 0, Edge nodes = 0)
        {
                sequence = (sequence << 1) | Edge(branch);      // shift-feedback register
                int edge = sequence & edge_mask;                // the last N+1 bits of the sequence

                auto visited = Edge(1) << edge;                 // the index of the edge
                if (tour & visited) return 1;                   // Eulerian tours visit each edge exactly once

                if (!depth) {
                        //std::cout << "sequence: " << std::hex << uint64_t(sequence >> N) << '\n';
                        return 1;
                }

                tour |= visited;                                // add the current edge to the tour
                vertex = edge >> 1;                             // the last N bits of the sequence
                nodes |= Edge(1) << vertex;
                /*
                if (depth < N+1)
                        return Eulerian(vertex, 0, depth - 1, sequence, tour, nodes);

                if (depth == N+1) {
                        //if (nodes == all_vertices)
                        //std::cout << int(nodes) << " " << all_vertices << '\n';
                        //if (nodes == all_vertices)
                                return Eulerian(vertex, 1, depth - 1, sequence, tour, nodes);
                        //else
                        //        return 0;
                }
                */
                return (
                        Eulerian(vertex, 1, depth - 1, sequence, tour, nodes) +
                        Eulerian(vertex, 0, depth - 1, sequence, tour, nodes)
                );
        }
};

template<typename T>
class Index
{
public:
        int operator()(T b)
        {
                b *= Sequence<T>::value;
                b >>= shift;
                return Table<T>::value[b];
        }

private:
        BOOST_STATIC_CONSTANT(auto, N = log2_sizeof<T>::value);         // log base-2 of number of bits
        BOOST_STATIC_CONSTANT(auto, shift = num_bits<T>::value - N);    // 2^N - N
};

}       // namespace debruijn
}       // namespace bit
}       // namespace dctl

// The remainder of this file has been copied from the output of the following main program:

/*
#include "../src/bit/DeBruijn.h"

int main()
{
        using namespace dctl::bit;
        DeBruijn<uint8_t >::generate_table();
        DeBruijn<uint16_t>::generate_table();
        DeBruijn<uint32_t>::generate_table();
        DeBruijn<uint64_t>::generate_table();
}
*/

/*

        // compute the lexicographically least De Bruijn sequence
        static T generate_sequence()
        {
                T sequence(0);

                // highest N bits are always zero; only have to try the remaining bits (SHIFT == 2^N - N)
                for (auto i = SHIFT - 1; i >= 0; --i) {
                        // cast the intermediate expression back to <T> to suppress promotion of small integer types
                        auto new_prefix = static_cast<T>(sequence << (SHIFT - i)) >> SHIFT;

                        // greedy algorithm: set the least significant bit of the next prefix equal to one
                        new_prefix ^= T(1);

                        // test if the new prefix matches any of the previous prefixes
                        bool prefer_one = true;
                        for (auto j = SHIFT - 1; j > i; --j) {
                                // cast the intermediate expression back to <T> to suppress promotion of small integer types
                                auto old_prefix = static_cast<T>(sequence << (SHIFT - j)) >> SHIFT;
                                if (new_prefix == old_prefix) {
                                        prefer_one = false;
                                        break;
                                }
                        }

                        // if the new prefix did not occur before, add a one bit to the sequence
                        if (prefer_one)
                                sequence ^= singlet<T>(i);
                }

                return sequence;
        }

        // compute the prefix table and print to stdout
        static void generate_table()
        {
                auto const sequence = generate_sequence();

                int table[NUM_BITS];
                for (auto i = 0; i < NUM_BITS; ++i) {
                        T b = singlet<T>(i);
                        b *= sequence;
                        b >>= SHIFT;
                        table[b] = i;
                }

                std::cout << "template<>\n";
                std::cout << "const " << typeid(T).name() << " DeBruijn<" << typeid(T).name() << ">::SEQUENCE = ";
                std::cout << "0x" << std::hex << std::setw(NUM_BITS / 4) << std::setfill('0');
                std::cout << static_cast<uint64_t>(sequence) << ";\n\n";

                std::cout << "template<>\n";
                std::cout << "int const DeBruijn<" << typeid(T).name() << ">::TABLE[] = {\n";
                for (auto i = 0; i < NUM_BITS; ++i) {
                        if (i % 8 == 0)
                                for (auto j = 0; j < 8; ++j)
                                        std::cout << " ";
                        std::cout << std::dec << std::setw(2) << std::setfill(' ') << table[i];
                        if (i != NUM_BITS - 1)
                                std::cout << ",";
                        if (i % 8 == 7 || i == NUM_BITS - 1)
                                std::cout << "\n";
                        else
                                std::cout << " ";
                }
                std::cout << "};" << "\n\n";
        }
*/
