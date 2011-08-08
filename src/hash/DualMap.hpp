#include "DualMap.h"
#include "../node/Position.h"

namespace dctl {
namespace hash {

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
DualMap<Key, Value, Hash, Index, Replace>::DualMap(std::size_t log2_n)
{
        resize(log2_n);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
std::size_t DualMap<Key, Value, Hash, Index, Replace>::available() const
{
        return dual_map_[0].available() + dual_map_[1].available();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
std::size_t DualMap<Key, Value, Hash, Index, Replace>::size() const
{
        return dual_map_[0].size() + dual_map_[1].size();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void DualMap<Key, Value, Hash, Index, Replace>::resize(std::size_t log2_n)
{
        assert(log2_n > 0);
        dual_map_[0].resize(log2_n - 1);
        dual_map_[1].resize(log2_n - 1);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
void DualMap<Key, Value, Hash, Index, Replace>::clear()
{
        dual_map_[0].clear();
        dual_map_[1].clear();
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Board>
const Value* DualMap<Key, Value, Hash, Index, Replace>::find(const Position<Board>& p) const
{
        return dual_map_[p.active_color()].find(p);
}

template<typename Key, typename Value, template<typename, typename> class Hash, typename Index, typename Replace>
template<typename Board>
void DualMap<Key, Value, Hash, Index, Replace>::insert(const Position<Board>& p, const Value& value)
{
        dual_map_[p.active_color()].insert(p, value);
}

}       // namespace hash
}       // namespace dctl
