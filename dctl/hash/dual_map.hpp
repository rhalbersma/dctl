#pragma once
#include <cstddef>                      // size_t
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/hash/hash_extractor.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/parity_extractor.hpp>
#include <dctl/hash/replace.hpp>

namespace dctl {
namespace hash {

template
<
        typename KeyExtractor,
        typename T,
        typename Parity = ActiveColorExtractor,
        typename Container = Map< KeyExtractor, T, EmptyOldUnderCutMin<Smallest> >
>
struct DualMap
{
private:
        // typedefs

        typedef Container map_type;
        typedef typename map_type::size_type size_type;
        typedef typename map_type::mapped_pointer mapped_pointer;
        typedef typename map_type::const_mapped_pointer const_mapped_pointer;

public:
        // structors

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
                return (map_[0].empty() && map_[1].empty());
        }

        size_type size() const
        {
                return (map_[0].size() + map_[1].size());
        }

        size_type max_size() const
        {
                return (map_[0].max_size() + map_[1].max_size());
        }

        size_type capacity() const
        {
                return (map_[0].capacity() + map_[1].capacity());
        }

        // modifiers

        void clear()
        {
                map_[0].clear();
                map_[1].clear();
        }

        template<typename U>
        void insert(U const& u, T const& t)
        {
                map_[Parity()(u)].insert(u, t);
        }

        void resize(size_type mega_bytes)
        {
                map_[0].resize(mega_bytes >> 1);
                map_[1].resize(mega_bytes >> 1);
        }

        // lookup

        template<typename U>
        mapped_pointer find(U const& u)
        {
                return (map_[Parity()(u)].find(u));
        }

        template<typename U>
        const_mapped_pointer find(U const& u) const
        {
                return (map_[Parity()(u)].find(u));
        }

private:
        // representation

        map_type map_[2];
};

}       // namespace hash
}       // namespace dctl
