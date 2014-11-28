#pragma once
#include <dctl/random/uniform.hpp>      // cat_zero_runif
#include <array>                        // array
#include <cstdint>                      // uint64_t

namespace dctl {
namespace zobrist {

template<class = void>
struct ActiveColor
{
        static std::array<uint64_t, 2> const color;
};

template<class _>
std::array<uint64_t, 2> const
ActiveColor<_>::color = random::cat_zero_runif<1>();

}       // namespace zobrist
}       // namespace dctl
