#pragma once
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace dctl {
namespace hash {

template<typename Key, typename Value, int N>
struct find_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::const_iterator ConstIterator;

        const Value* operator()(ConstIterator, const Key&) const;
};

template<typename Key, typename Value, int N, typename Replacement>
struct insert_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator, const Entry&) const;
};

}       // namespace hash
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "FindInsert.hpp"
