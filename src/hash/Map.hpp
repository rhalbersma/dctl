#pragma once
#include <algorithm>                    // count_if, fill_n
#include <cstddef>                      // size_t
#include <type_traits>                  // integral_constant, is_integral, false_type, true_type
#include <utility>                      // pair
#include <vector>                       // vector
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "FindInsert.hpp"
#include "Functions.hpp"
#include "KeySign.hpp"
#include "Replace.hpp"
#include "../utility/CacheAlign.hpp"
#include "../utility/IntegerTypes.hpp"

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
        // structors

        Map()
        {
                resize(0);
        }

        explicit Map(std::size_t log2_n)
        {
                resize(log2_n);
        }

        // capacity

        std::size_t available() const
        {
                return static_cast<size_t>(
                        std::count_if(
                                std::begin(map_), std::end(map_), 
                                [](Entry const& e) 
                                { return e.first == Key(0); }
                        )
                );
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
                std::fill_n(std::begin(map_), map_.size(), Entry(Key(0), Value()));
        }

        // queries

        Value const* find(Key const& key) const
        {
                auto const index = Hash<Key, Index>()(key);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        template<typename Item>
        Value const* find(Item const& item) const
        {
                // tag dispatching on the key's integral type trait
                return find_dispatch(
                        item,
                        std::integral_constant<bool, std::is_integral<Key>::value>()
                );
        }

        // modifiers

        void insert(Key const& key, Value const& value)
        {
                auto const index = Hash<Key, Index>()(key);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }

        template<typename Item>
        void insert(Item const& item, Value const& value)
        {
                // tag dispatching on the key's integral type trait
                insert_dispatch(
                        item, value,
                        std::integral_constant<bool, std::is_integral<Key>::value>()
                );
        }

private:
        // partial specialization for non-integral keys
        template<typename Item>
        Value const* find_dispatch(Item const& item, std::false_type) const
        {
                auto const index = Hash<Index, Item>()(item);
                auto const key = FindKey<Key, Item>()(item);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        // partial specialization for integral keys
        template<typename Item>
        Value const* find_dispatch(Item const& item, std::true_type) const
        {
                auto const index = Hash<Index, Item>()(item);
                auto const key = ShiftKey<Key, Index>()(index);
                return find_entry<Key, Value, bucket_size>()(bucket_begin(index), key);
        }

        // partial specialization for non-integral keys
        template<typename Item>
        void insert_dispatch(Item const& item, Value const& value, std::false_type)
        {
                auto const index = Hash<Index, Item>()(item);
                auto const key = FindKey<Key, Item>()(item);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }

        // partial specialization for integral keys
        template<typename Item>
        void insert_dispatch(Item const& item, Value const& value, std::true_type)
        {
                auto const index = Hash<Index, Item>()(item);
                auto const key = ShiftKey<Key, Index>()(index);
                insert_entry<Key, Value, bucket_size, Replace>()(bucket_begin(index), Entry(key, value));
        }

        typedef std::pair<Key, Value> Entry;
        typedef std::vector<Entry> VectorMap;
        typedef typename VectorMap::iterator map_iterator;
        typedef typename VectorMap::const_iterator const_map_iterator;

        static std::size_t const bucket_size = CACHE_LINE / sizeof(Entry);
        static std::size_t const bucket_mask = bucket_size - 1;

        map_iterator bucket_begin(Index index)
        {
                return std::begin(map_) + static_cast<std::size_t>(index & map_mask_);
        }

        const_map_iterator bucket_begin(Index index) const
        {
                return std::begin(map_) + static_cast<std::size_t>(index & map_mask_);
        }

        // representation
        VectorMap map_;
        std::size_t map_mask_;
};

}       // namespace hash
}       // namespace dctl
