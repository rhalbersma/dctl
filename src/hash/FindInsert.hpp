#include <algorithm>    // std::find_if, std::get_first_of, std::min_element
#include <functional>   // std::bind
#include "EntryPredicates.h"
#include "Replace.h"

namespace dctl {
namespace hash {

template<typename Key, typename Value, int N>
const Value* find_entry<Key, Value, N>::operator()(ConstIterator bucket_begin, const Key& key) const
{
        auto bucket_end = bucket_begin + N;
        auto entry = std::find_if(bucket_begin, bucket_end, std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, key));
        return (entry != bucket_end)? &(entry->second) : nullptr;
}

// partial specialization for the following 3-stage replacement scheme
template<typename Key, typename Value, int N>
struct insert_entry<Key, Value, N, EmptyOldUnderCutSmallestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;
        typedef typename std::vector<Entry>::iterator Iterator;

        void operator()(Iterator bucket_begin, const Entry& entry) const
        {
                auto bucket_end = bucket_begin + N;

                // replace any empty or old entry
                Key empty_or_old[2] = {Key(0), entry.first};
                auto replace = std::find_first_of(bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
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
                Key empty_or_old[2] = {Key(0), entry.first};
                auto replace = std::find_first_of(bucket_begin, bucket_end, empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
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
}       // namespace dctl
