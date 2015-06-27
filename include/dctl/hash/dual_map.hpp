#pragma once
#include <cstddef>                      // size_t
#include <dctl/hash/extract.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/replace.hpp>

namespace dctl {
namespace hash {

template
<
        class Key,
        class T,
        std::size_t N,
        class Tag,
        class Parity = extract::ActivePlayer,
        class Container = set_associative_cache<Key, T, N, EmptyOldMin<Smallest>, Tag>
>
struct dual_set_associative_cache
{
private:
        using map_type = Container;
        using size_type = typename map_type::size_type;
        using iterator = typename map_type::iterator;
        using const_iterator = typename map_type::const_iterator;
        using mapped_pointer = typename map_type::mapped_pointer;
        using const_mapped_pointer = typename map_type::const_mapped_pointer;

private:
        map_type map_[2];
public:
        explicit dual_set_associative_cache(size_type sz)
        {
                resize(sz);
        }

        dual_set_associative_cache()
        {
                resize(2);
        }

        // capacity

        bool empty() const
        {
                return map_[0].empty() && map_[1].empty();
        }

        size_type size() const
        {
                return map_[0].size() + map_[1].size();
        }

        size_type max_size() const
        {
                return map_[0].max_size() + map_[1].max_size();
        }

        size_type capacity() const
        {
                return map_[0].capacity() + map_[1].capacity();
        }

        // modifiers

        auto clear()
        {
                map_[0].clear();
                map_[1].clear();
        }

        auto resize(size_type sz)
        {
                map_[0].resize(sz >> 1);
                map_[1].resize(sz >> 1);
        }

        auto insert(Key const& key, T const& value)
        {
                /*return*/ map_[Parity{}(key)].insert(key, value);
        }

        // lookup

        auto find(Key const& key)
        {
                return map_[Parity{}(key)].find(key);
        }

        auto find(Key const& key) const
        {
                return map_[Parity{}(key)].find(key);
        }
};

}       // namespace hash
}       // namespace dctl
