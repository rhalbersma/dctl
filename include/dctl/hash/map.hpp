#pragma once
#include <algorithm>                    // count_if, fill_n
#include <array>                        // array
#include <cstddef>                      // ptrdiff_t, size_t
#include <functional>                   // equal_to, hash
#include <memory>                       // allocator
#include <type_traits>                  // integral_constant, is_integral, false_type, true_type
#include <utility>                      // make_pair, pair
#include <vector>                       // vector
#include <dctl/hash/extract.hpp>

namespace dctl {
namespace hash {

template
<
        class Key,
        class T,
        class Signature,
        class Replace,
        class Hash = extract::Hash,
        class KeyEqual = std::equal_to<typename Signature::result_type>,
        class Allocator = std::allocator< std::pair<typename Signature::result_type, T> >
>
struct Map
{
public:
        using key_type = typename Signature::result_type;
        using mapped_type = T;
        using value_type = std::pair<key_type, mapped_type>;
        using size_type = std::size_t;
        using differrence_type = std::ptrdiff_t;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using allocator = Allocator;
        using reference = value_type&;
        using const_reference = value_type const&;

public:
        using index_type = typename hasher::result_type;
        using iterator = value_type*;
        using const_iterator = value_type const*;
        using mapped_pointer = mapped_type*;
        using const_mapped_pointer = mapped_type const*;

private:
        static auto const N = 64 / sizeof(value_type);
        using bucket_type = std::array<value_type, N>;
        static_assert(sizeof(bucket_type) == 64, "non-aligned hash table");
        using map_type = std::vector<bucket_type>;

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
                for (auto& b : map_) b.fill(value_type(key_type(0), mapped_type()));
                size_ = 0;
        }

        void resize(size_type mega_bytes)
        {
                auto const n = (mega_bytes << 20) / sizeof(bucket_type);
                map_.resize(n);
                mask_ = map_.size() - 1;
                clear();
        }

        void insert(Key const& key, T const& t)
        {
                auto const index = hasher()(key);
                auto const first = begin(bucket(index));
                auto const last = first + bucket_size();

                auto const insertion = Replace()(first, last, std::make_pair(Signature()(/*key,*/ index), t));
                size_ += insertion;
                //return insertion;
        }

        // lookup

        mapped_pointer find(Key const& key)
        {
                auto const index = hasher()(key);
                auto const first = begin(bucket(index));
                auto const last = first + bucket_size();

                auto it = std::find_if(first, last, [&](auto const& entry) {
                        return key_equal{}(entry.first, Signature()(/*key,*/ index));
                });
                return (it != last) ? &(it->second) : nullptr;
        }

private:
        // bucket interface
        using local_iterator = typename bucket_type::iterator;
        using const_local_iterator = typename bucket_type::const_iterator;

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
