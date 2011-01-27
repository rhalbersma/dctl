#include "DualMap.h"
#include "../Position/Position.h"

namespace Hash {

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
DualMap<Key, Value, Replace, Hash, Index>::DualMap(size_t log2_n)
{
        assert(log2_n > 0);
        resize(log2_n);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
void DualMap<Key, Value, Replace, Hash, Index>::resize(size_t log2_n)
{
        dual_map_[0].resize(log2_n - 1);
        dual_map_[1].resize(log2_n - 1);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
const Value* DualMap<Key, Value, Replace, Hash, Index>::find(const Position<Board>& p) const
{
        return dual_map_[p.to_move()].find(p);
}

template<typename Key, typename Value, typename Replace, template<typename, typename> class Hash, typename Index>
template<typename Board>
void DualMap<Key, Value, Replace, Hash, Index>::insert(const Position<Board>& p, const Value& value)
{
        dual_map_[p.to_move()].insert(p, value);
}

}       // namespace Hash