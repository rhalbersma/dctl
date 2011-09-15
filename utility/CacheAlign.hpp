#pragma once
#include "IntegerTypes.hpp"

namespace dctl {

// a cache line is 64 bytes on a P4
static const auto LOG2_CACHE_LINE = 6;
static const auto CACHE_LINE = 1 << LOG2_CACHE_LINE;

#define CACHE_ALIGN __declspec(align(64))

}       // namespace dctl
