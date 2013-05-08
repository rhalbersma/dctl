#pragma once
#include <algorithm>                    // min_element
#include <iterator>                     // begin, end, iterator_traits
#include <type_traits>                  // is_same
#include <utility>                      // pair, make_pair
#include <tuple>
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
                for (auto it = first; it != last; ++it) {
                        if (it->first == key_type(0)) {
                                *it = value;
                                return true;
                        }

                        if (it->first == value.first) {
                                it->second = value.second;
                                return false;
                        }
                }

                // 2) replace the first entry if its depth is under cut by one
                if (value.second.depth() == first->second.depth() - 1) {
                        *first = value;
                        return false;
                }

                // 3) replace the minimal entry with respect to the Predicate
                auto it = std::min_element(first, last, Predicate());
                *it = value;
                return false;
        }
};

struct Shallowest
{
        template<typename T>
        bool operator()(T const& lhs, T const& rhs) const
        {
                return lhs.second.depth() < rhs.second.depth();
        }
};

struct Smallest
{
        template<typename T>
        bool operator()(T const& lhs, T const& rhs) const
        {
                return lhs.second.nodes() < rhs.second.nodes();
        }
};

}       // namespace hash
}       // namespace dctl
