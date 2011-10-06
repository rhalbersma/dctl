#pragma once
#include <cstddef>                      // std::size_t
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Functions.hpp"
#include "Map.hpp"
#include "Replace.hpp"
#include "../node/Position.hpp"

namespace dctl { 
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
        explicit DualMap(std::size_t log2_n)
        {
                resize(log2_n);
        }

        // capacity
        std::size_t available() const
        {
                return dual_map_[0].available() + dual_map_[1].available();
        }

        std::size_t size() const
        {
                return dual_map_[0].size() + dual_map_[1].size();
        }

        void resize(std::size_t log2_n)
        {
                BOOST_ASSERT(log2_n > 0);
                dual_map_[0].resize(log2_n - 1);
                dual_map_[1].resize(log2_n - 1);
        }

        void clear()
        {
                dual_map_[0].clear();
                dual_map_[1].clear();
        }

        // views
        template<typename Board> 
        const Value* find(const Position<Board>& p) const
        {
                return dual_map_[p.active_color()].find(p);
        }

        // modifiers
        template<typename Board> 
        void insert(const Position<Board>& p, const Value& value)
        {
                dual_map_[p.active_color()].insert(p, value);
        }

private:
        // representation
        Map<Key, Value, Hash, Index, Replace> dual_map_[2];
};

}       // namespace hash
}       // namespace dctl
