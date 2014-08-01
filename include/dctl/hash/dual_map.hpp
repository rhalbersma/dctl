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
        class Signature,
        class Parity = extract::ActiveColor,
        class Container = Map< Key, T, Signature, EmptyOldUnderCutMin<Smallest> >
>
struct DualMap
{
private:
        using map_type = Container;
        using size_type = typename map_type::size_type;
        using iterator = typename map_type::iterator;
        using const_iterator = typename map_type::const_iterator;
        using mapped_pointer = typename map_type::mapped_pointer;
        using const_mapped_pointer = typename map_type::const_mapped_pointer;

public:
        // constructors

        DualMap()
        {
                resize(2);
        }

        explicit DualMap(size_type mega_bytes)
        {
                resize(mega_bytes);
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

        void clear()
        {
                map_[0].clear();
                map_[1].clear();
        }

        void resize(size_type mega_bytes)
        {
                map_[0].resize(mega_bytes >> 1);
                map_[1].resize(mega_bytes >> 1);
        }

        void insert(Key const& key, T const& value)
        {
                /*return*/ map_[Parity()(key)].insert(key, value);
        }

        // lookup

        mapped_pointer find(Key const& key)
        {
                return map_[Parity()(key)].find(key);
        }

        const_mapped_pointer find(Key const& key) const
        {
                return map_[Parity()(key)].find(key);
        }

private:
        // representation

        map_type map_[2];
};

}       // namespace hash
}       // namespace dctl
