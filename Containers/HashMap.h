#pragma once
#include "../Utilities/CacheAlign.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Hash/HashReplace.h"
#include "../Hash/HashAlgorithms.h"
#include <array>
#include <utility>
#include <vector>

template
<
        typename Key,
        typename Value,
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = ZobristFind,
        typename Index = HashIndex
>
class HashMap
{
public:
        // constructors
        HashMap(void);
	explicit HashMap(size_t);

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
        // tag dispatching based on the key's integer type trait
        template<typename Item> const Value* find(const Item&, Int2Type<true>) const;
        template<typename Item> const Value* find(const Item&, Int2Type<false>) const;

        // tag dispatching based on the key's integer type trait
        template<typename Item> void insert(const Item&, const Value&, Int2Type<true>);
        template<typename Item> void insert(const Item&, const Value&, Int2Type<false>);

        size_t bucket(Index) const;

        typedef std::pair<Key, Value> Entry;
        static const size_t ASSOCIATIVITY = CACHE_LINE / sizeof(Entry);
        typedef std::tr1::array<Entry, ASSOCIATIVITY> Bucket;

        static const size_t LOG_MEGA_BYTE = 20;
        static const size_t LOG_GIGA_BYTE = 30;
        static const size_t MIN_LOG_BUCKETS = LOG_MEGA_BYTE - LOG_CACHE_LINE;
        static const size_t MAX_LOG_BUCKETS = LOG_GIGA_BYTE - LOG_CACHE_LINE;

        // representation
        std::vector<Bucket> hash_map_;
        Index bucket_mask_;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "HashMap.hpp"
