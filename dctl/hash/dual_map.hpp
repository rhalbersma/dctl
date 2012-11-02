#pragma once
#include <cstddef>                      // size_t
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/hash/function.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/replace.hpp>

namespace dctl {
namespace hash {

template
<
        typename Key,
        typename Value,
        template<typename, typename> class Hash = Find,
        typename Index = HashIndex,
        typename Replace = EmptyOldUnderCutSmallestOfN
>
struct DualMap
{
public:
        // structors

        DualMap()
        {
                resize(1);
        }

        explicit DualMap(std::size_t log2_n)
        {
                resize(log2_n);
        }

        // capacity

        std::size_t available() const
        {
                return (dual_map_[0].available() + dual_map_[1].available());
        }

        std::size_t size() const
        {
                return (dual_map_[0].size() + dual_map_[1].size());
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

        // queries

        template<typename Position>
        Value const* find(Position const& p) const
        {
                return (dual_map_[p.active_color()].find(p));
        }

        // modifiers

        template<typename Position>
        void insert(Position const& p, Value const& value)
        {
                dual_map_[p.active_color()].insert(p, value);
        }

private:
        // representation

        typedef Map<Key, Value, Hash, Index, Replace> map_type;
        typedef typename map_type::Entry Entry;
        map_type dual_map_[2];
};

}       // namespace hash
}       // namespace dctl