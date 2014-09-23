#pragma once
#include <dctl/bitset/intrinsic/builtin.hpp>      // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using gcc built-in functions
#include <dctl/bitset/intrinsic/lookup.hpp>       // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using table lookup
#include <dctl/bitset/intrinsic/loop.hpp>         // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using brute-force looping

namespace xstd {
namespace intrinsic {

namespace lib = builtin;
using namespace lib;

}       // namespace intrinsic
}       // namespace xstd
