#pragma once
#include <cstddef>      // std::size_t
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace tree {
namespace hash {

template<typename Key, typename Value, std::size_t N>
struct find_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::const_iterator ConstIterator;

        const Value* operator()(ConstIterator, const Key&) const;
};

template<typename Key, typename Value, std::size_t N, typename Replacement>
struct insert_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator, const Entry&) const;
};

}       // namespace hash
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "FindInsert.hpp"
