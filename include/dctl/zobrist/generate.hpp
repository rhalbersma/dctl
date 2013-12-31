#pragma once
#include <cstdint>                      // uint64_t
#include <random>                       // mt19937, uniform_int_distribution

namespace dctl {
namespace zobrist {

// dummy int parameter to allow calls from make_array<N>
inline
uint64_t generate(int /* n */)
{
        static std::mt19937 engine;
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
}

inline
uint64_t zero_generate(int n)
{
        return n ? generate(n) : 0x0000000000000000;
}

}       // namespace zobrist
}       // namespace dctl
