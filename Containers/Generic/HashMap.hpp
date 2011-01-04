#include "../../Hash/HashSign.h"

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
HashMap<Key, Value, LogN, Replace, Hash, Index>::HashMap(void)
{
        hash_map_.reserve(NUM_BUCKETS);
}

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
const Value* HashMap<Key, Value, LogN, Replace, Hash, Index>::find(const Key& key) const
{
        const Index index = Hash<Key, Index>()(key);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, LogN, Replace, Hash, Index>::find(const Item& item) const
{
        return find(item, Int2Type<is_IntegerType<Key>::VALUE>());
}

// partial specialization for integer keys
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, LogN, Replace, Hash, Index>::find(const Item& item, Int2Type<true>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

// partial specialization for non-integer keys
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
const Value* HashMap<Key, Value, LogN, Replace, Hash, Index>::find(const Item& item, Int2Type<false>) const
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        return find_entry<Key, Value>()(hash_map_[bucket(index)], key);
}

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
void HashMap<Key, Value, LogN, Replace, Hash, Index>::insert(const Key& key, const Value& value)
{
        const Index index = Hash<Key, Index>()(key);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

// tag dispatching based on the key's integer type trait
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, LogN, Replace, Hash, Index>::insert(const Item& item, const Value& value)
{
        insert(item, value, Int2Type<is_IntegerType<Key>::VALUE>());
}

// partial specialization for integer keys
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, LogN, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<true>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = ShiftSign<Index, Key>()(index);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

// partial specialization for non-integer keys
template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Item>
void HashMap<Key, Value, LogN, Replace, Hash, Index>::insert(const Item& item, const Value& value, Int2Type<false>)
{
        const Index index = Hash<Item, Index>()(item);
        const Key key = FindSign<Item, Key>()(item);
        insert_entry<Key, Value, Replace>()(hash_map_[bucket(index)], Entry(key, value));
}

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
size_t HashMap<Key, Value, LogN, Replace, Hash, Index>::bucket(Index index)
{
	return static_cast<size_t>(index & BUCKET_MASK);
}
