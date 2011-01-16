#include "../Hash/HashSign.h"
#include <cassert>
#include <iostream>

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
HashMap<Key, Value, Replace, Hash, Index>::HashMap(size_t log2_n)
{
        resize(log2_n);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void HashMap<Key, Value, Replace, Hash, Index>::resize(size_t log2_n)
{
        size_t log2_b = log2_n + MIN_LOG2_BUCKETS;
        while (log2_b >= MIN_LOG2_BUCKETS) {
                try {
                        hash_map_.resize(Index(1) << log2_b);   // try to allocate all the buckets
                }
                catch (const std::bad_alloc&) {                        
                        --log2_b;                               // try allocating half the previous size
                        continue;       
                }
                bucket_mask_ = hash_map_.size() - 1;            // mask to do arithmetic MODULO the number of buckets
                std::cout << "Successfully allocated " << hash_map_.size() << " buckets " << std::endl;
                return;
        }
        throw;                                                  // could not allocate the minimum number of buckets
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
const Value* HashMap<Key, Value, Replace, Hash, Index>::find(const Key& key) const
{
        const Index index = Hash<Key, Index>()(key);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, Replace, Hash, Index>::find(const Item& item) const
{
        return find(item, Int2Type<is_IntegerType<Key>::VALUE>());
}

// partial specialization for integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, Replace, Hash, Index>::find(const Item& item, Int2Type<true>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

// partial specialization for non-integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, Replace, Hash, Index>::find(const Item& item, Int2Type<false>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void HashMap<Key, Value, Replace, Hash, Index>::insert(const Key& key, const Value& value)
{
        const Index index = Hash<Key, Index>()(key);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value)
{
        insert(item, value, Int2Type<is_IntegerType<Key>::VALUE>());
}

// partial specialization for integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<true>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

// partial specialization for non-integer keys
template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<false>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
size_t HashMap<Key, Value, Replace, Hash, Index>::bucket(Index index) const
{
	return static_cast<size_t>(index & bucket_mask_);
}
