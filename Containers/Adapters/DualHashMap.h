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
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = ZobristFind,
        typename Index = HashIndex
>
class DualHashMap
{
public:
        DualHashMap(size_t);

        void resize(size_t);

        // views
        template<typename B> const Value* find(const Position<B>&) const;

        // modifiers
        template<typename B> void insert(const Position<B>&, const Value&);

private:
        // representation
        HashMap<Key, Value, Replace, Hash, Index> hash_map_[2];
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DualHashMap.hpp"
