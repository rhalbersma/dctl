#include "DualHashMap.h"
#include "../../Position/Position.h"

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
const Value* DualHashMap<Key, Value, LogN, Replace, Hash, Index>::find(const Position<Board>& p) const
{
        return d_hash_map[p.to_move()].find(p);
}

template<typename Key, typename Value, size_t LogN, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
void DualHashMap<Key, Value, LogN, Replace, Hash, Index>::insert(const Position<Board>& p, const Value& value)
{
        d_hash_map[p.to_move()].insert(p, value);
}
