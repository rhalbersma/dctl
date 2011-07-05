#pragma once
#include <cstddef>
#include "Map.h"
#include "Replace.h"
#include "Algorithms.h"

namespace tree { namespace node { template<typename> class Position; } }

namespace hash {

template
<
        typename Key,
        typename Value,
        template<typename, typename> class Hash = zobrist::Find,
        typename Index = HashIndex,
        typename Replace = EmptyOldUnderCutShallowestOfN
>
class DualMap
{
public:
        // constructors
        explicit DualMap(size_t);

        // capacity
        size_t size() const;
        size_t available() const;
        void resize(size_t);
        void clear();

        // views
        template<typename Board> const Value* find(const tree::node::Position<Board>&) const;

        // modifiers
        template<typename Board> void insert(const tree::node::Position<Board>&, const Value&);

private:
        // representation
        Map<Key, Value, Hash, Index, Replace> dual_map_[2];
};

}

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DualMap.hpp"
