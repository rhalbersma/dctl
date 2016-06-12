#pragma once
#include <experimental/type_traits>     // is_same
#include <algorithm>                    // min_element
#include <iterator>                     // iterator_traits

namespace dctl {
namespace hash {

template<class Predicate>
struct EmptyOldMin
{
        template<class ForwardIterator, class Value>
        bool operator()(ForwardIterator first, ForwardIterator last, Value const& value) const
        {
                static_assert(std::experimental::is_same_v<typename std::iterator_traits<ForwardIterator>::value_type, Value>);
                using tag_type = typename Value::first_type;

                // 1) fill an empty slot or replace an existing entry with the same tag
                for (auto it = first; it != last; ++it) {
                        if (it->first == tag_type{0}) {
                                *it = value;
                                return true;
                        }

                        if (it->first == value.first) {
                                it->second = value.second;
                                return false;
                        }
                }

                // 2) replace the minimal entry with respect to the Predicate
                auto it = std::min_element(first, last, Predicate());
                *it = value;
                return false;
        }
};

struct Shallowest
{
        template<class T>
        constexpr auto operator()(T const& lhs, T const& rhs) const noexcept
        {
                return lhs.second.depth() < rhs.second.depth();
        }
};

struct Smallest
{
        template<class T>
        constexpr auto operator()(T const& lhs, T const& rhs) const noexcept
        {
                return lhs.second.nodes() < rhs.second.nodes();
        }
};

}       // namespace hash
}       // namespace dctl
