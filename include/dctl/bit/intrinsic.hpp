#pragma once
#include <dctl/bit/intrinsic/builtin.hpp>       // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using gcc built-in functions
#include <dctl/bit/intrinsic/lookup.hpp>        // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using table lookup
#include <dctl/bit/intrinsic/loop.hpp>          // ctznz, clznz, ctz, clz, bsfnz, bsrnz, popcount using brute-force looping

namespace dctl {
namespace bit {
namespace intrinsic {

namespace lib = builtin;
using namespace lib;

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
