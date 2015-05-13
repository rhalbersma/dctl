#pragma once
#include <algorithm>    // find_if
#include <array>        // array
#include <cstddef>      // size_t
#include <functional>   // equal_to, hash
#include <memory>       // allocator, allocator_traits
#include <utility>      // make_pair, pair
#include <vector>       // vector

namespace dctl {
namespace hash {

template
<
        class Key,
        class T,
        std::size_t N,
        class Replace,
        class Tag,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<typename Tag::result_type>,
        class Allocator = std::allocator<std::pair<typename Tag::result_type, T>>
>
class set_associative_cache
{
        static_assert(N && !(N & (N - 1)), "Set associativity should be a power of 2.");
public:
        using key_type          = Key;
        using tag_type          = typename Tag::result_type;
        using value_type        = std::pair<tag_type, T>;
        using mapped_type       = T;
        using tagger            = Tag;
        using hasher            = Hash;
        using tag_equal         = Pred;
        using allocator_type    = Allocator;
        using pointer           = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer     = typename std::allocator_traits<Allocator>::const_pointer;
        using reference         = value_type&;
        using const_reference   = value_type const&;

        using size_type         = typename std::vector<value_type>::size_type;
        using differrence_type  = typename std::vector<value_type>::difference_type;
        using mapped_pointer            = mapped_type*;
        using const_mapped_pointer      = mapped_type const*;

private:
        using set_type = std::array<value_type, N>;
        static_assert(sizeof(set_type) == 64, "non-aligned hash table");

        std::vector<set_type> data_;
        size_type mask_;
        size_type size_;

public:
        explicit set_associative_cache(size_type mega_bytes)
        {
                resize(mega_bytes);
        }

        set_associative_cache()
        {
                resize(1);
        }

        size_type size() const noexcept
        {
                return size_;
        }

        size_type max_size() const noexcept
        {
                return max_set_count() * set_associativity();
        }

        void resize(size_type sz)
        {
                auto const n = (sz << 20) / sizeof(set_type);
                data_.resize(n);
                mask_ = set_count() - 1;
                clear();
        }

        size_type capacity() const noexcept
        {
                return set_count() * set_associativity();
        }

        bool empty() const noexcept
        {
                return size() != 0;
        }

        void clear()
        {
                for (auto& b : data_)
                        b.fill(value_type{tag_type{0}, mapped_type{}});
                size_ = 0;
        }

        void insert(Key const& key, T const& t)
        {
                auto const address = hasher{}(key);
                auto const first = begin(data_[set_index(address)]);
                auto const last = first + set_associativity();

                auto const insertion = Replace{}(first, last, std::make_pair(tagger{}(key, address), t));
                size_ += insertion;
        }

        auto find(Key const& key) const
        {
                auto const address = hasher{}(key);
                auto const first = cbegin(data_[set_index(address)]);
                auto const last = first + set_associativity();

                auto const it = std::find_if(first, last, [&](auto const& block) {
                        return tag_equal{}(block.first, tagger{}(key, address));
                });
                return (it != last) ? &(it->second) : nullptr;
        }

private:
        auto set_count() const noexcept
        {
                return data_.size();
        }

        auto max_set_count() const noexcept
        {
                return data_.max_size();
        }

        auto set_associativity() const noexcept
        {
                return N;
        }

        auto set_index(std::size_t address) const noexcept
        {
                return static_cast<size_type>(address) & mask_; // % set_count()
        }
};

}       // namespace hash
}       // namespace dctl
