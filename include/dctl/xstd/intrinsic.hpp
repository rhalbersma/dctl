#pragma once
#include <dctl/xstd/intrinsic/builtin.hpp>      // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using gcc built-in functions
#include <dctl/xstd/intrinsic/lookup.hpp>       // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using table lookup
#include <dctl/xstd/intrinsic/loop.hpp>         // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using brute-force looping

namespace xstd {
namespace intrinsic {

namespace lib = builtin;
using namespace lib;

}       // namespace intrinsic
}       // namespace xstd
