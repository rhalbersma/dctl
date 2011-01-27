#pragma once

#ifdef _MSC_VER
#include <array>
#else
#include <tr1/array>
#endif

#include <utility>

namespace Hash {

// replacement policies
struct EmptyOldUnderCutSmallestOfN {};
struct EmptyOldUnderCutShallowestOfN {};

template<typename Key, typename Value>
struct find_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;

        template<size_t N>
        const Value* operator()(const std::tr1::array<Entry, N>&, const Key&) const;
};

template<typename Key, typename Value, typename Replacement>
struct insert_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;

        template<size_t N>
        void operator()(std::tr1::array<Entry, N>&, const Entry&) const;
};

}       // namespace Hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Replace.hpp"