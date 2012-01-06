#pragma once
#include <algorithm>                    // std::find_if, std::get_first_of, std::min_element
#include <functional>                   // std::bind
#include <utility>                      // std::pair
#include <vector>                       // std::vector
#include "EntryPredicates.hpp"
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
                const auto entry = std::find_if(
                        bucket_begin, bucket_end, 
                        std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, key)
                );
                return (entry != bucket_end)? &(entry->second) : nullptr;
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
                auto replace = std::find_first_of(
                        bucket_begin, bucket_end, 
                        empty_or_old, empty_or_old + 2, 
                        key_equal_to<Entry, Key>()
                );
                if (replace != bucket_end) {
                        *replace = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket_begin->second.depth() - 1) {
                        *bucket_begin = entry;
                        return;
                }

                // replace the smallest entry
                replace = std::min_element(
                        bucket_begin, bucket_end, 
                        leafs_compare<Entry>()
                );
                *replace = entry;
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
                auto replace = std::find_first_of(
                        bucket_begin, bucket_end, 
                        empty_or_old, empty_or_old + 2, 
                        key_equal_to<Entry, Key>()
                );
                if (replace != bucket_end) {
                        *replace = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket_begin->second.depth() - 1) {
                        *bucket_begin = entry;
                        return;
                }

                // replace the shallowest entry
                replace = std::min_element(
                        bucket_begin, bucket_end, 
                        depth_compare<Entry>()
                );
                *replace = entry;
        }
};

}       // namespace hash
}       // namespace dctl
