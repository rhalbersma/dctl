#pragma once
#include <algorithm>                    // std::count_if, std::fill_n
#include <cstddef>                      // std::size_t
#include <functional>                   // std::bind, std::placeholders
#include <type_traits>                  // std::is_integral
#include <utility>                      // std::pair
#include <vector>                       // std::vector
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "FindInsert.hpp"
#include "Functions.hpp"
#include "KeySign.hpp"
#include "Replace.hpp"
#include "../utility/CacheAlign.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {
namespace hash {

template
<
        typename Key,
        typename Value,
        template<typename, typename> class Hash = Find,
        typename Index = HashIndex,
        typename Replace = EmptyOldUnderCutShallowestOfN
>
class Map
{
public:
        // constructors
        Map() { /* no-op */ }
        explicit Map(std::size_t log2_n)
        {
                resize(log2_n);
        }

        // capacity
        std::size_t available() const
        {
                return static_cast<size_t>(std::count_if(
                        map_.begin(), map_.end(), 
                        std::bind(key_equal_to<Entry, Key>(), std::placeholders::_1, Key(0))
                ));
        }

        std::size_t size() const
        {
                return map_.size();
        }

        void resize(std::size_t log2_n)
        {
                map_.resize(Index(1) << log2_n);        // allocate all the entries
                map_mask_ = map_.size() - 1;            // modulo the number of entries
                map_mask_ ^= bucket_mask;               // modulo the number of buckets
                clear();
        }

        void clear()
        {
                std::fill_n(map_.begin(), map_.size(), Entry(Key(0), Value()));
        }

        // views
        const Value* find(const Key& key) const
        {
                const auto index = Hash<Key, Index>()(key);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        // tag dispatching on the key's integral type trait
        template<typename Item>
        const Value* find(const Item& item) const
        {
                return find(
                        item, 
                        Int2Type<std::is_integral<Key>::value>()
                );
        }

        // modifiers
        void insert(const Key& key, const Value& value)
        {
                const auto index = Hash<Key, Index>()(key);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }

        // tag dispatching on the key's integral type trait
        template<typename Item>
        void insert(const Item& item, const Value& value)
        {
                insert(
                        item, value, 
                        Int2Type<std::is_integral<Key>::value>()
                );
        }

private:
        // partial specialization for non-integral keys
        template<typename Item> 
        const Value* find(const Item& item, Int2Type<false>) const
        {
                const auto index = Hash<Item, Index>()(item);
                const auto key = FindKey<Item, Key>()(item);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        // partial specialization for integral keys
        template<typename Item> 
        const Value* find(const Item& item, Int2Type<true>) const
        {
                const auto index = Hash<Item, Index>()(item);
                const auto key = ShiftKey<Index, Key>()(index);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        // partial specialization for non-integral keys
        template<typename Item> 
        void insert(const Item& item, const Value& value, Int2Type<false>)
        {
                const auto index = Hash<Item, Index>()(item);
                const auto key = FindKey<Item, Key>()(item);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }
        
        // partial specialization for integral keys
        template<typename Item> 
        void insert(const Item& item, const Value& value, Int2Type<true>)
        {
                const auto index = Hash<Item, Index>()(item);
                const auto key = ShiftKey<Index, Key>()(index);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }

        typedef std::pair<Key, Value> Entry;
        typedef std::vector<Entry> VectorMap;
        typedef typename VectorMap::iterator map_iterator;
        typedef typename VectorMap::const_iterator const_map_iterator;

        static const std::size_t bucket_size = CACHE_LINE / sizeof(Entry);
        static const std::size_t bucket_mask = bucket_size - 1;
        
        map_iterator bucket_begin(Index index)
        {
	        return map_.begin() + static_cast<std::size_t>(index & map_mask_);
        }

        const_map_iterator bucket_begin(Index index) const
        {
	        return map_.begin() + static_cast<std::size_t>(index & map_mask_);
        }

        // representation
        VectorMap map_;
        std::size_t map_mask_;
};

}       // namespace hash
}       // namespace dctl
