#include "DualMap.h"
#include "../Tree/Node/Position.h"

namespace hash {

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
DualMap<Key, Value, Hash, Index, Replace>::DualMap(size_t log2_n)
{
        assert(log2_n > 0);
        resize(log2_n);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
size_t DualMap<Key, Value, Hash, Index, Replace>::size(void) const
{
        return dual_map_[0].size() + dual_map_[1].size();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
size_t DualMap<Key, Value, Hash, Index, Replace>::empty(void) const
{
        return dual_map_[0].empty() + dual_map_[1].empty();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void DualMap<Key, Value, Hash, Index, Replace>::resize(size_t log2_n)
{
        dual_map_[0].resize(log2_n - 1);
        dual_map_[1].resize(log2_n - 1);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void DualMap<Key, Value, Hash, Index, Replace>::clear(void)
{
        dual_map_[0].clear();
        dual_map_[1].clear();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Board>
const Value* DualMap<Key, Value, Hash, Index, Replace>::find(const tree::node::Position<Board>& p) const
{
        return dual_map_[p.to_move()].find(p);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Board>
void DualMap<Key, Value, Hash, Index, Replace>::insert(const tree::node::Position<Board>& p, const Value& value)
{
        dual_map_[p.to_move()].insert(p, value);
}

}       // namespace hash
