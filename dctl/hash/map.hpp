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
#include <dctl/hash/index_extractor.hpp>
#include <dctl/hash/replace.hpp>
#include <dctl/utility/cache_align.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace hash {

template
<
        typename SignatureExtractor,
        typename T,
        typename Replace = EmptyOldUnderCutMin<Shallowest>,
        typename Hash = IndexExtractor
>
struct Map
{
private:
        typedef typename SignatureExtractor::value_type Signature;

public:
        typedef Signature key_type;
        typedef T mapped_type;
        typedef std::pair<Signature, T> value_type;

private:
        static auto const N = CACHE_LINE / sizeof(value_type);
        typedef std::array<value_type, N> bucket_type;
        static_assert(sizeof(bucket_type) == CACHE_LINE, "non-aligned hash table");

        typedef typename Hash::value_type index_type;
        typedef std::vector<bucket_type> map_type;

public:
        typedef std::size_t size_type;
        typedef value_type* iterator;
        typedef value_type const* const_iterator;
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

        template<typename Key>
        bool insert(Key const& key, T const& t)
        {
            	auto const index = Hash()(key);
            	auto const signature = SignatureExtractor()(key, index);
                auto const n = bucket(index);
                auto const insertion = Replace()(begin(n), end(n), std::make_pair(signature, t));
                size_ += insertion;
                return insertion;
        }

        template<typename Key, typename... Args>
        bool emplace(Key const& key, Args&&... args)
        {
                auto const index = Hash()(key);
                auto const signature = SignatureExtractor()(key, index);
                auto const n = bucket(index);
                auto const emplacement = Replace()(begin(n), end(n), std::make_pair(signature, T(std::forward<Args>(args)...)));
                size_ += emplacement;
                return emplacement;
        }

        void resize(size_type mega_bytes)
        {
                auto const n = (mega_bytes << 20) / sizeof(bucket_type);
                map_.resize(n);
                mask_ = map_.size() - 1;
                clear();
        }

        // lookup

        template<typename Key>
        mapped_pointer find(Key const& key)
        {
            	auto const index = Hash()(key);
            	auto const signature = SignatureExtractor()(key, index);
                auto const n = bucket(index);

                auto it = std::find_if(
                        begin(n), end(n),
                        [&](value_type const& e) {
                        return e.first == signature;
                });
                return (it != end(n))? &(it->second) : nullptr;
        }

        template<typename Key>
        const_mapped_pointer find(Key const& key) const
        {
            	auto const index = Hash()(key);
            	auto const signature = SignatureExtractor()(key, index);
                auto const n = bucket(index);

                auto it = std::find_if(
                        cbegin(n), cend(n),
                        [&](value_type const& e) {
                        return e.first == signature;
                });
                return (it != cend(n))? &(it->second) : nullptr;
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

        template<typename Key>
        size_type bucket(Key const& key) const
        {
                return bucket(Hash()(key));
        }

        size_type bucket(index_type index) const
        {
                return static_cast<size_type>(index) & mask_;
        }

        // representation

        map_type map_;
        size_type mask_;
        size_type size_;
};

}       // namespace hash
}       // namespace dctl
