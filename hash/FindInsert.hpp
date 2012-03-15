#pragma once
#include <algorithm>                    // find_if, get_first_of, min_element
#include <utility>                      // pair
#include <vector>                       // vector
#include "Replace.hpp"

namespace dctl {
namespace hash {

template<typename Key, typename Value, int N>
struct find_entry
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::const_iterator ConstIterator;

        const Value* operator()(ConstIterator bucket_begin, const Key& key) const
        {
                const auto bucket_end = bucket_begin + N;
                const auto it = std::find_if(
                        bucket_begin, bucket_end, [&](const Entry& entry){
                        return entry.first == key;
                });
                return (it != bucket_end)? &(it->second) : nullptr;
        }
};

// primary template for hash key insertion
template<typename Key, typename Value, int N, typename Replacement>
struct insert_entry;

// partial specialization for the following 3-stage replacement scheme
template<typename Key, typename Value, int N>
struct insert_entry<Key, Value, N, EmptyOldUnderCutSmallestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator bucket_begin, const Entry& entry) const
        {
                const auto bucket_end = bucket_begin + N;

                // replace any empty or old entry
                Key empty_or_old[] = { Key(0), entry.first };
                auto it = std::find_first_of(
                        bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, [](const Entry& entry, const Key& key) {
                        return entry.first == key;
                });
                if (it != bucket_end) {
                        *it = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket_begin->second.depth() - 1) {
                        *bucket_begin = entry;
                        return;
                }

                // replace the smallest entry
                it = std::min_element(
                        bucket_begin, bucket_end, [](const Entry& lhs, const Entry& rhs) {
                        return lhs.second.leafs() < rhs.second.leafs();
                });
                *it = entry;
        }
};

// partial specialization for the following 3-stage replacement scheme
template<typename Key, typename Value, int N>
struct insert_entry<Key, Value, N, EmptyOldUnderCutShallowestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator bucket_begin, const Entry& entry) const
        {
                auto bucket_end = bucket_begin + N;

                // replace any empty or old entry
                Key empty_or_old[] = { Key(0), entry.first };
                auto it = std::find_first_of(
                        bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, [](const Entry& entry, const Key& key) {
                        return entry.first == key;
                });
                if (it != bucket_end) {
                        *it = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket_begin->second.depth() - 1) {
                        *bucket_begin = entry;
                        return;
                }

                // replace the shallowest entry
                it = std::min_element(
                        bucket_begin, bucket_end, [](const Entry& lhs, const Entry& rhs) {
                        return lhs.second.depth() < rhs.second.depth();
                });
                *it = entry;
        }
};

}       // namespace hash
}       // namespace dctl
