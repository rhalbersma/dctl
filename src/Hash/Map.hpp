#include <algorithm>    // std::count_if
#include <cassert>
#include <cstring>      // std::memset
#include <type_traits>  // std::is_integral

#include "FindInsert.h"
#include "Sign.h"

namespace hash {

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
Map<Key, Value, Hash, Index, Replace>::Map(size_t log2_n)
{
        resize(log2_n);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
size_t Map<Key, Value, Hash, Index, Replace>::size(void) const
{
        return map_.size();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
size_t Map<Key, Value, Hash, Index, Replace>::available(void) const
{
        return std::count_if(map_.begin(), map_.end(), std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, Key(0)));
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void Map<Key, Value, Hash, Index, Replace>::resize(size_t log2_n)
{
        while (log2_n >= 0) {
                try {
                        map_.resize(Index(1) << log2_n);        // try to allocate all the entries
                }
                catch (const std::bad_alloc&) {                                
                        --log2_n;                               // try allocating half the previous size
                        continue;       
                }
                map_mask_ = map_.size() - 1;                    // MODULO the number of entries
                map_mask_ ^= BUCKET_MASK;                       // MODULO the number of buckets
                return;
        }
        throw;                                                  // could not allocate a single entry
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void Map<Key, Value, Hash, Index, Replace>::clear(void)
{
        std::memset(&map_[0], 0, map_.size() * sizeof(Entry));
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
const Value* Map<Key, Value, Hash, Index, Replace>::find(const Key& key) const
{
        const auto index = Hash<Key, Index>()(key);
        return find_entry<Key, Value, BUCKET_SIZE>()(bucket_begin(index), key);
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
const Value* Map<Key, Value, Hash, Index, Replace>::find(const Item& item) const
{
        return find(item, Int2Type<std::is_integral<Key>::value>());
}

// partial specialization for integer keys
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
const Value* Map<Key, Value, Hash, Index, Replace>::find(const Item& item, Int2Type<true>) const
{
        const auto index = Hash<Item, Index>()(item);
        const auto key = ShiftSign<Index, Key>()(index);
        return find_entry<Key, Value, BUCKET_SIZE>()(bucket_begin(index), key);
}

// partial specialization for non-integer keys
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
const Value* Map<Key, Value, Hash, Index, Replace>::find(const Item& item, Int2Type<false>) const
{
        const auto index = Hash<Item, Index>()(item);
        const auto key = FindSign<Item, Key>()(item);
        return find_entry<Key, Value, BUCKET_SIZE>()(bucket_begin(index), key);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void Map<Key, Value, Hash, Index, Replace>::insert(const Key& key, const Value& value)
{
        const auto index = Hash<Key, Index>()(key);
        insert_entry<Key, Value, BUCKET_SIZE, Replace>()(bucket_begin(index), Entry(key, value));
}

// tag dispatching based on the key's integral type trait
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
void Map<Key, Value, Hash, Index, Replace>::insert(const Item& item, const Value& value)
{
        insert(item, value, Int2Type<std::is_integral<Key>::value>());
}

// partial specialization for integral keys
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
void Map<Key, Value, Hash, Index, Replace>::insert(const Item& item, const Value& value, Int2Type<true>)
{
        const auto index = Hash<Item, Index>()(item);
        const auto key = ShiftSign<Index, Key>()(index);
        insert_entry<Key, Value, BUCKET_SIZE, Replace>()(bucket_begin(index), Entry(key, value));
}

// partial specialization for non-integral keys
template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Item>
void Map<Key, Value, Hash, Index, Replace>::insert(const Item& item, const Value& value, Int2Type<false>)
{
        const auto index = Hash<Item, Index>()(item);
        const auto key = FindSign<Item, Key>()(item);
        insert_entry<Key, Value, BUCKET_SIZE, Replace>()(bucket_begin(index), Entry(key, value));
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
typename Map<Key, Value, Hash, Index, Replace>::map_iterator Map<Key, Value, Hash, Index, Replace>::bucket_begin(Index index)
{
	return map_.begin() + static_cast<size_t>(index & map_mask_);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
typename Map<Key, Value, Hash, Index, Replace>::const_map_iterator Map<Key, Value, Hash, Index, Replace>::bucket_begin(Index index) const
{
	return map_.begin() + static_cast<size_t>(index & map_mask_);
}

}       // namespace hash
