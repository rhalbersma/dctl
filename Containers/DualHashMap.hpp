#include "DualHashMap.h"
#include "../Position/Position.h"

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
DualHashMap<Key, Value, Replace, Hash, Index>::DualHashMap(size_t log_n)
{
        assert(log_n > 0);
        resize(log_n);        
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void DualHashMap<Key, Value, Replace, Hash, Index>::resize(size_t log_n)
{
        hash_map_[0].resize(log_n - 1);
        hash_map_[1].resize(log_n - 1);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
const Value* DualHashMap<Key, Value, Replace, Hash, Index>::find(const Position<Board>& p) const
{
        return hash_map_[p.to_move()].find(p);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
void DualHashMap<Key, Value, Replace, Hash, Index>::insert(const Position<Board>& p, const Value& value)
{
        hash_map_[p.to_move()].insert(p, value);
}
