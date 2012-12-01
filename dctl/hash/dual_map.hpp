#pragma once
#include <cstddef>                      // size_t
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/hash/index_extractor.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/parity_extractor.hpp>
#include <dctl/hash/replace.hpp>

namespace dctl {
namespace hash {

template
<
        typename SignatureExtractor,
        typename T,
        typename Parity = ActiveColorExtractor,
        typename Container = Map< SignatureExtractor, T, EmptyOldUnderCutMin<Smallest> >
>
struct DualMap
{
private:
        // typedefs

        typedef Container map_type;
        typedef typename map_type::size_type size_type;
        typedef typename map_type::iterator iterator;
        typedef typename map_type::const_iterator const_iterator;
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

        template<typename Key>
        bool insert(Key const& key, T const& value)
        {
                return map_[Parity()(key)].insert(key, value);
        }

        template<typename Key, typename... Args>
        bool emplace(Key const& key, Args&&... args)
        {
        	return map_[Parity()(key)].emplace(key, std::forward<Args>(args)...);
        }

        void resize(size_type mega_bytes)
        {
                map_[0].resize(mega_bytes >> 1);
                map_[1].resize(mega_bytes >> 1);
        }

        // lookup

        template<typename Key>
        mapped_pointer find(Key const& key)
        {
                return map_[Parity()(key)].find(key);
        }

        template<typename Key>
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
