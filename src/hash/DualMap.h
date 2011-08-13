#pragma once
#include <cstddef>      // std::size_t
#include "Algorithms.h"
#include "Map.h"
#include "Replace.h"

namespace dctl { 
        
template<typename> class Position;

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
        explicit DualMap(std::size_t);

        // capacity
        std::size_t available() const;
        std::size_t size() const;
        void resize(std::size_t);
        void clear();

        // views
        template<typename Board> const Value* find(const Position<Board>&) const;

        // modifiers
        template<typename Board> void insert(const Position<Board>&, const Value&);

private:
        // representation
        Map<Key, Value, Hash, Index, Replace> dual_map_[2];
};

}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "DualMap.hpp"
