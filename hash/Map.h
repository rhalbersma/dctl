#pragma once
#include <cstddef>      // std::size_t
#include <utility>      // std::pair
#include <vector>       // std::vector
#include "Algorithms.h"
#include "Replace.h"
#include "../utility/CacheAlign.h"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {
namespace hash {

template
<
        typename Key,
        typename Value,
        template<typename, typename> class Hash = zobrist::Find,
        typename Index = HashIndex,
        typename Replace = EmptyOldUnderCutShallowestOfN
>
class Map
{
public:
        // constructors
        Map() {};
        explicit Map(std::size_t);

        // capacity
        std::size_t available() const;
        std::size_t size() const;
        void resize(std::size_t);
        void clear();

        // views
        const Value* find(const Key&) const;

        template<typename Item>
        const Value* find(const Item&) const;

        // modifiers
        void insert(const Key&, const Value&);

        template<typename Item>
        void insert(const Item&, const Value&);

private:
        // tag dispatching based on the key's integer type trait
        template<typename Item> const Value* find(const Item&, Int2Type<false>) const;
        template<typename Item> const Value* find(const Item&, Int2Type<true >) const;

        // tag dispatching based on the key's integer type trait
        template<typename Item> void insert(const Item&, const Value&, Int2Type<false>);
        template<typename Item> void insert(const Item&, const Value&, Int2Type<true >);

        typedef std::pair<Key, Value> Entry;
        typedef std::vector<Entry> VectorMap;
        typedef typename VectorMap::iterator map_iterator;
        typedef typename VectorMap::const_iterator const_map_iterator;

        static const std::size_t BUCKET_size = CACHE_LINE / sizeof(Entry);
        static const std::size_t BUCKET_MASK = BUCKET_size - 1;
        
              map_iterator bucket_begin(Index);
        const_map_iterator bucket_begin(Index) const;

        // representation
        VectorMap map_;
        std::size_t map_mask_;
};

}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "Map.hpp"
