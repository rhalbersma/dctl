#pragma once
#include <dctl/util/units.hpp>
#include <xstd/cstdlib.hpp>      // is_power_of
#include <algorithm>    // find_if
#include <array>        // array
#include <cassert>      // assert
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
        static_assert(xstd::is_power_of_2(N));
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
        size_type size_;

public:
        explicit set_associative_cache(size_type sz)
        {
                assert(xstd::is_power_of_2(sz));
                resize(sz);
        }

        set_associative_cache()
        {
                resize(64_KiB);
        }

        auto size() const noexcept
        {
                return size_;
        }

        auto max_size() const noexcept
        {
                return max_set_count() * N;
        }

        auto resize(size_type sz)
        {
                assert(xstd::is_power_of_2(sz));
                data_.resize(sz);
                clear();
        }

        auto capacity() const noexcept
        {
                return set_count() * N;
        }

        auto empty() const noexcept
        {
                return size() != 0;
        }

        auto clear()
        {
                for (auto& b : data_)
                        b.fill(value_type{tag_type{0}, mapped_type{}});
                size_ = 0;
        }

        auto insert(Key const& key, T const& t)
        {
                auto const address = hasher{}(key);
                auto const first = begin(data_[index(address)]);
                auto const last = first + N;

                auto const insertion = Replace{}(first, last, std::make_pair(tagger{}(key, address), t));
                size_ += insertion;
        }

        auto find(Key const& key) const
        {
                auto const address = hasher{}(key);
                auto const first = cbegin(data_[index(address)]);
                auto const last = first + N;

                auto const it = std::find_if(first, last, [&](auto const& block) {
                        return tag_equal{}(block.first, tagger{}(key, address));
                });
                return it != last ? &(it->second) : nullptr;
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

        auto index(std::size_t address) const noexcept
        {
                return static_cast<size_type>(address) & (set_count() - 1); // % set_count()
        }
};

}       // namespace hash
}       // namespace dctl
