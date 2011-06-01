#include <algorithm>
#include <functional>
#include "EntryPredicates.h"

namespace Hash {

template<typename Key, typename Value>
template<size_t N>
const Value* find_entry<Key, Value>::operator()(const std::tr1::array<Entry, N>& bucket, const Key& key) const
{
        typename std::tr1::array<Entry, N>::const_iterator entry = std::find_if(bucket.begin(), bucket.end(), std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, key));
        return (entry != bucket.end())? &(entry->second) : 0;
}

template<typename Key, typename Value>
struct insert_entry<Key, Value, EmptyOldUnderCutSmallestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;

        template<size_t N>
        void operator()(std::tr1::array<Entry, N>& bucket, const Entry& entry) const
        {
                typename std::tr1::array<Entry, N>::iterator replace;

                // replace any empty or old entry
                Key empty_or_old[2] = {Key(0), entry.first};
                replace = std::find_first_of(bucket.begin(), bucket.end(), empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
                if (replace != bucket.end()) {
                        *replace = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket.begin()->second.depth() - 1) {
                        *bucket.begin() = entry;
                        return;
                }

                // replace the smallest entry
                replace = std::min_element(bucket.begin(), bucket.end(), leafs_compare<Entry>());
                *replace = entry;
        }
};

template<typename Key, typename Value>
struct insert_entry<Key, Value, EmptyOldUnderCutShallowestOfN>
{
        // typedefs
        typedef std::pair<Key, Value> Entry;

        template<size_t N>
        void operator()(std::tr1::array<Entry, N>& bucket, const Entry& entry) const
        {
                typename std::tr1::array<Entry, N>::iterator replace;

                // replace any empty or old entry
                Key empty_or_old[2] = {Key(0), entry.first};
                replace = std::find_first_of(bucket.begin(), bucket.end(), empty_or_old, empty_or_old + 2, key_equal_to<Entry, Key>());
                if (replace != bucket.end()) {
                        *replace = entry;
                        return;
                }

                // replace the first entry if the new depth undercuts the old depth by one
                if (entry.second.depth() == bucket.begin()->second.depth() - 1) {
                        *bucket.begin() = entry;
                        return;
                }

                // replace the shallowest entry
                replace = std::min_element(bucket.begin(), bucket.end(), depth_compare<Entry>());
                *replace = entry;
        }
};

}       // namespace Hash
