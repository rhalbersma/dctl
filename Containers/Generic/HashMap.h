#pragma once
#include "../../Utilities/CacheAlign.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Hash/HashReplace.h"
#include "../../Hash/HashAlgorithms.h"
#include <array>
#include <utility>
#include <vector>

template
<
        typename Key,
        typename Value,
        size_t LogN,
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = ZobristFind,
        typename Index = HashIndex
>
class HashMap
{
public:
	HashMap(void);

        // views
        const Value* find(const Key&) const;

        template<typename Item>
        const Value* find(const Item&) const;

        // modifiers
        void insert(const Key&, const Value&);

        template<typename Item>
        void insert(const Item&, const Value&);

private:
        // implementation

        // tag dispatching based on the key's integer type trait
        template<typename Item> const Value* find(const Item&, Int2Type<true>) const;
        template<typename Item> const Value* find(const Item&, Int2Type<false>) const;

        // tag dispatching based on the key's integer type trait
        template<typename Item> void insert(const Item&, const Value&, Int2Type<true>);
        template<typename Item> void insert(const Item&, const Value&, Int2Type<false>);

        static size_t bucket(Index);

        typedef std::pair<Key, Value> Entry;

        static const size_t ASSOCIATIVITY = CACHE_LINE / sizeof(Entry);
        static const Index NUM_ENTRIES = Index(1) << LogN;
        static const Index NUM_BUCKETS = NUM_ENTRIES / ASSOCIATIVITY;
        static const Index BUCKET_MASK = NUM_BUCKETS - 1;

        typedef std::tr1::array<Entry, ASSOCIATIVITY> Bucket;

        // representation
        std::vector<Bucket> hash_map_;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "HashMap.hpp"
