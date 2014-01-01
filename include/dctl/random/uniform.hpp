#pragma once
#include <cstdint>                      // uint64_t
#include <random>                       // mt19937, uniform_int_distribution
#include <dctl/utility/make_array.hpp>  // array, make_array

namespace dctl {
namespace random {

inline
uint64_t xxx()
{
        static std::mt19937 engine;
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
}

inline
uint64_t generate(int)
{
        return xxx();
}

inline
uint64_t zero_generate(int n)
{
        return n ? xxx() : 0x0000000000000000;
}

template<int N>
auto runif()
{
        return make_array<N>(generate);
}

template<int N>
auto cat_zero_runif()
{
        return make_array<1 + N>(zero_generate);
}

}       // namespace random
}       // namespace dctl
