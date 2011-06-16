#include <algorithm>
#include <functional>
#include "EntryPredicates.h"

namespace hash {

template<typename Key, typename Value, size_t N>
const Value* find_entry<Key, Value, N>::operator()(ConstIterator bucket_begin, const Key& key) const
{
        ConstIterator bucket_end = bucket_begin + N;
        ConstIterator entry = std::find_if(bucket_begin, bucket_end, std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, key));
        return (entry != bucket_end)? &(entry->second) : 0;
}

template<typename Key, typename Value, size_t N>
struct insert_entry<Key, Value, N, EmptyOldUnderCutSmallestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator bucket_begin, const Entry& entry) const
        {
                Iterator replace;
                Iterator bucket_end = bucket_begin + N;

                // replace any empty or old entry
                Key empty_or_old[2] = {Key(0), entry.first};
                replace = std::find_first_of(bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
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
                replace = std::min_element(bucket_begin, bucket_end, leafs_compare<Entry>());
                *replace = entry;
        }
};

template<typename Key, typename Value, size_t N>
struct insert_entry<Key, Value, N, EmptyOldUnderCutShallowestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator bucket_begin, const Entry& entry) const
        {
                Iterator replace;
                Iterator bucket_end = bucket_begin + N;

                // replace any empty or old entry
                Key empty_or_old[2] = {Key(0), entry.first};
                replace = std::find_first_of(bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
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
                replace = std::min_element(bucket_begin, bucket_end, depth_compare<Entry>());
                *replace = entry;
        }
};

}       // namespace hash
