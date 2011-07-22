#pragma once
#include "IntegerTypes.h"

namespace dctl {

// a cache line is 64 bytes on a P4
static const size_t LOG2_CACHE_LINE = 6;
static const size_t CACHE_LINE = 1 << LOG2_CACHE_LINE;

#define CACHE_ALIGN __declspec(align(64))

}       // namespace dctl
