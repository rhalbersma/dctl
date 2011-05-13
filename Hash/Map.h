#pragma once
#include "../Utilities/CacheAlign.h"
#include "../Utilities/TemplateTricks.h"
#include "Algorithms.h"
#include "Replace.h"

#ifdef _MSC_VER
#include <array>
#else
#include <tr1/array>
#endif

#include <cstddef>
#include <utility>
#include <vector>

namespace Hash {

template
<
        typename Key,
        typename Value,
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = Zobrist::Find,
        typename Index = HashIndex
>
class Map
{
public:
        // constructors
        Map(void) {};
        explicit Map(size_t);

        // capacity
        void resize(size_t);

        // views
        const Value* find(const Key&) const;

        template<typename Item>
        const Value* find(const Item&) const;

        // modifiers
        void insert(const Key&, const Value&);

        template<typename Item>
        void insert(const Item&, const Value&);

private:
        static const size_t LOG2_MEGA_BYTE = 20;
        static const size_t LOG2_GIGA_BYTE = 30;
        static const size_t MIN_LOG2_BUCKETS = LOG2_MEGA_BYTE - LOG2_CACHE_LINE;
        static const size_t MAX_LOG2_BUCKETS = LOG2_GIGA_BYTE - LOG2_CACHE_LINE;

        // tag dispatching based on the key's integer type trait
        template<typename Item> const Value* find(const Item&, Int2Type<true >) const;
        template<typename Item> const Value* find(const Item&, Int2Type<false>) const;

        // tag dispatching based on the key's integer type trait
        template<typename Item> void insert(const Item&, const Value&, Int2Type<true >);
        template<typename Item> void insert(const Item&, const Value&, Int2Type<false>);

        size_t bucket(Index) const;

        typedef std::pair<Key, Value> Entry;
        static const size_t ASSOCIATIVITY = CACHE_LINE / sizeof(Entry);
        typedef std::tr1::array<Entry, ASSOCIATIVITY> Bucket;

        // representation
        std::vector<Bucket> map_;
        Index bucket_mask_;
};

}       // namespace Hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Map.hpp"
