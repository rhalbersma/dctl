#pragma once
#include <algorithm>                    // count_if, fill_n
#include <array>                        // array
#include <cstddef>                      // ptrdiff_t, size_t
#include <functional>                   // equal_to, hash
#include <memory>                       // allocator
#include <type_traits>                  // integral_constant, is_integral, false_type, true_type
#include <utility>                      // make_pair, pair
#include <vector>                       // vector
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/hash/hash_extractor.hpp>
#include <dctl/hash/replace.hpp>
#include <dctl/utility/cache_align.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace hash {

template
<
        typename KeyExtractor,
        typename T,
        typename Hash = HashExtractor,
        typename Replace = EmptyOldUnderCutMin<Shallowest>
>
struct Map
{
private:
        typedef typename KeyExtractor::key_type Key;

public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<Key, T> value_type;

private:
        static auto const N = CACHE_LINE / sizeof(value_type);
        typedef std::array<value_type, N> bucket_type;
        static_assert(sizeof(bucket_type) == CACHE_LINE, "non-aligned hash table");

        typedef std::vector<bucket_type> map_type;

public:
        typedef std::size_t size_type;
        typedef mapped_type* mapped_pointer;
        typedef mapped_type const* const_mapped_pointer;

public:
        // structors

        Map()
        {
                resize(1);
        }

        explicit Map(size_type mega_bytes)
        {
                resize(mega_bytes);
        }

        // capacity

        bool empty() const
        {
                return size() != 0;
        }

        size_type size() const
        {
                return size_;
        }

        size_type max_size() const
        {
                return max_bucket_count() * bucket_size();
        }

        size_type capacity() const
        {
                return bucket_count() * bucket_size();
        }

        // modifiers

        void clear()
        {
                std::memset(&map_[0], 0, map_.size() * sizeof(bucket_type));
                size_ = 0;
        }

        template<typename U>
        void insert(U const& u, T const& t)
        {
                auto const hash = Hash()(u);
                auto const key = KeyExtractor()(u, hash);
                auto const n = index(hash);
                size_ += Replace()(begin(n), end(n), std::make_pair(key, t));
        }

        void resize(size_type mega_bytes)
        {
                auto const n = (mega_bytes << 20) / sizeof(bucket_type);
                map_.resize(n);
                mask_ = map_.size() - 1;
                clear();
        }

        // lookup

        template<typename U>
        mapped_pointer find(U const& u)
        {
                auto const hash = Hash()(u);
                auto const key = KeyExtractor()(u, hash);
                auto const n = index(hash);

                auto it = std::find_if(
                        begin(n), end(n),
                        [&](value_type const& e) {
                        return (e.first == key);
                });
                return ((it != end(n))? &(it->second) : nullptr);
        }

        template<typename U>
        const_mapped_pointer find(U const& u) const
        {
                auto const hash = Hash()(u);
                auto const key = KeyExtractor()(u, hash);
                auto const n = index(hash);

                auto it = std::find_if(
                        cbegin(n), cend(n),
                        [&](value_type const& e) {
                        return (e.first == key);
                });
                return ((it != cend(n))? &(it->second) : nullptr);
        }

private:
        // bucket interface

        typedef typename bucket_type::iterator local_iterator;
        typedef typename bucket_type::const_iterator const_local_iterator;

        local_iterator begin(size_type n)
        {
                return map_[n].begin();
        }

        const_local_iterator begin(size_type n) const
        {
                return map_[n].begin();
        }

        const_local_iterator cbegin(size_type n) const
        {
                return map_[n].cbegin();
        }

        local_iterator end(size_type n)
        {
                return map_[n].end();
        }

        const_local_iterator end(size_type n) const
        {
                return map_[n].end();
        }

        const_local_iterator cend(size_type n) const
        {
                return map_[n].cend();
        }

        size_type bucket_count() const
        {
                return map_.size();
        }

        size_type max_bucket_count() const
        {
                return map_.max_size();
        }

        size_type bucket_size(size_type /* n */ = 0) const
        {
                return N;
        }

        template<typename U>
        size_type bucket(U const& u) const
        {
                return index(Hash()(u));
        }

        template<typename Size>
        size_type index(Size hash) const
        {
                return static_cast<size_type>(hash) & mask_;
        }

        // representation

        map_type map_;
        size_type mask_;
        size_type size_;
};

}       // namespace hash
}       // namespace dctl
