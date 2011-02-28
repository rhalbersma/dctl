#include "Sign.h"
#include <cassert>
#include <iostream>
#include <boost/type_traits.hpp>

namespace Hash {

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
Map<Key, Value, Replace, Hash, Index>::Map(size_t log2_n)
{
        resize(log2_n);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void Map<Key, Value, Replace, Hash, Index>::resize(size_t log2_n)
{
        size_t log2_b = log2_n + MIN_LOG2_BUCKETS;
        while (log2_b >= MIN_LOG2_BUCKETS) {
                try {
                        map_.resize(Index(1) << log2_b);        // try to allocate all the buckets
                }
                catch (const std::bad_alloc&) {                                
                        --log2_b;                               // try allocating half the previous size
                        continue;       
                }
                bucket_mask_ = map_.size() - 1;                 // mask to do arithmetic MODULO the number of buckets
                std::cout << "Successfully allocated " << map_.size() << " buckets " << std::endl;
                return;
        }
        throw;                                                  // could not allocate the minimum number of buckets
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
const Value* Map<Key, Value, Replace, Hash, Index>::find(const Key& key) const
{
        const Index index = Hash<Key, Index>()(key);
        return find_entry<Key, Value>()(map_[bucket(index)], key);
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* Map<Key, Value, Replace, Hash, Index>::find(const Item& item) const
{
        return find(item, Int2Type<boost::is_integral<Key>::value>());
}

// partial specialization for integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* Map<Key, Value, Replace, Hash, Index>::find(const Item& item, Int2Type<true>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        return find_entry<Key, Value>()(map_[bucket(index)], key);
}

// partial specialization for non-integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* Map<Key, Value, Replace, Hash, Index>::find(const Item& item, Int2Type<false>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        return find_entry<Key, Value>()(map_[bucket(index)], key);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void Map<Key, Value, Replace, Hash, Index>::insert(const Key& key, const Value& value)
{
        const Index index = Hash<Key, Index>()(key);
        insert_entry<Key, Value, Replace>()(map_[bucket(index)], Entry(key, value));
}

// tag dispatching based on the key's integral type trait
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void Map<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value)
{
        insert(item, value, Int2Type<boost::is_integral<Key>::value>());
}

// partial specialization for integral keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void Map<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<true>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        insert_entry<Key, Value, Replace>()(map_[bucket(index)], Entry(key, value));
}

// partial specialization for non-integral keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void Map<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<false>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        insert_entry<Key, Value, Replace>()(map_[bucket(index)], Entry(key, value));
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
size_t Map<Key, Value, Replace, Hash, Index>::bucket(Index index) const
{
	return static_cast<size_t>(index & bucket_mask_);
}

}       // namespace Hash
