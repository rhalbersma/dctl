#pragma once
#include <cstddef>
#include <utility>
#include <vector>

namespace hash {

// replacement policies
struct EmptyOldUnderCutSmallestOfN {};
struct EmptyOldUnderCutShallowestOfN {};

template<typename Key, typename Value, size_t N>
struct find_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::const_iterator ConstIterator;

        const Value* operator()(ConstIterator, const Key&) const;
};

template<typename Key, typename Value, size_t N, typename Replacement>
struct insert_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator, const Entry&) const;
};

}       // namespace hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Replace.hpp"
