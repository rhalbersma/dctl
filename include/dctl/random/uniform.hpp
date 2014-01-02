#pragma once
#include <cstdint>                      // uint64_t
#include <random>                       // mt19937, uniform_int_distribution
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace random {

inline
auto uniform_uint64()
{
        static std::mt19937 engine;
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
}

template<int N>
auto runif()
{
        return make_array<N>([](auto /* n */){
                return uniform_uint64();
        });
}

template<int N>
auto cat_zero_runif()
{
        return make_array<1 + N>([](auto n){
                return n ? uniform_uint64() : 0;
        });
}

}       // namespace random
}       // namespace dctl
