#pragma once

// a cache line is 64 bytes on a P4
static const size_t LOG_CACHE_LINE = 6;
static const size_t CACHE_LINE = 1 << LOG_CACHE_LINE;
