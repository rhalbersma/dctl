#pragma once
#include <algorithm>                    // find_if, get_first_of, min_element
#include <iterator>                     // begin, end, iterator_traits
#include <type_traits>                  // is_same
#include <utility>
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT

namespace dctl {
namespace hash {

template<typename Predicate>
struct EmptyOldUnderCutMin
{
        template<typename ForwardIterator, typename value_type>
        bool operator()(ForwardIterator first, ForwardIterator last, value_type const& value) const
        {
                BOOST_MPL_ASSERT(( std::is_same<typename std::iterator_traits<ForwardIterator>::value_type, value_type> ));
                typedef typename value_type::first_type key_type;

                // 1) fill an empty slot or replace an existing entry with the same key
                key_type slots[] = { key_type(0), value.first };
                auto it = std::find_first_of(
                        first, last,
                        std::begin(slots), std::end(slots),
                        [](value_type const& v, key_type const& k) {
                        return (v.first == k);
                });
                if (it != last) {
                        auto const insertion = it->first != value.first;
                        *it = value;
                        return (insertion);
                }

                // 2) replace the first entry if its depth is under cut by one
                if (value.second.depth() == first->second.depth() - 1) {
                        *first = value;
                        return (false);
                }

                // 3) replace the minimal entry with respect to the Predicate
                it = std::min_element(first, last, Predicate());
                *it = value;
                return (false);
        }
};

struct Shallowest
{
        template<typename T>
        bool operator()(T const& lhs, T const& rhs) const
        {
                return (lhs.second.depth() < rhs.second.depth());
        }
};

struct Smallest
{
        template<typename T>
        bool operator()(T const& lhs, T const& rhs) const
        {
                return (lhs.second.leafs() < rhs.second.leafs());
        }
};

}       // namespace hash
}       // namespace dctl
