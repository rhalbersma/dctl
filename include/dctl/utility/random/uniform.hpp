#pragma once
#include <dctl/utility/fill_array.hpp>  // fill_array
#include <cstdint>                      // uint64_t
#include <random>                       // mt19937_64, uniform_int_distribution

namespace dctl {
namespace random {

inline
auto uniform_uint64()
{
        static std::mt19937_64 engine;
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
}

template<int N>
auto runif()
{
        return fill_array<N>([](auto /* n */){
                return uniform_uint64();
        });
}

template<int N>
auto cat_zero_runif()
{
        return fill_array<1 + N>([](auto n){
                return n == 0 ? 0 : uniform_uint64();
        });
}

template<int N>
auto cat_runif_zero()
{
        return fill_array<1 + N>([](auto n){
                return n != N ? uniform_uint64() : 0;
        });
}

}       // namespace random
}       // namespace dctl
