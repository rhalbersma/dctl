#pragma once
#include "../Generic/HashMap.h"
#include "../../Hash/HashReplace.h"
#include "../../Hash/HashAlgorithms.h"
#include "../../Utilities/IntegerTypes.h"

template<typename> class Position;

template
<
        typename Key,
        typename Value,
        size_t LogN,
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = ZobristFind,
        typename Index = HashIndex
>
class DualHashMap
{
public:
        // views
        template<typename B> const Value* find(const Position<B>&) const;

        // modifiers
        template<typename B> void insert(const Position<B>&, const Value&);

private:
        // representation
        HashMap<Key, Value, LogN - 1, Replace, Hash, Index> d_hash_map[2];
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "DualHashMap.hpp"
