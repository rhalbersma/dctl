#pragma once
#include "Map.h"
#include "Replace.h"
#include "Algorithms.h"
#include <cstddef>

template<typename> class Position;

namespace Hash {

template
<
        typename Key,
        typename Value,
        typename Replace = EmptyOldUnderCutShallowestOfN,
        template<typename, typename> class Hash = Zobrist::Find,
        typename Index = HashIndex
>
class DualMap
{
public:
        DualMap(size_t);

        void resize(size_t);

        // views
        template<typename Board> const Value* find(const Position<Board>&) const;

        // modifiers
        template<typename Board> void insert(const Position<Board>&, const Value&);

private:
        // representation
        Map<Key, Value, Replace, Hash, Index> dual_map_[2];
};

}

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DualMap.hpp"
