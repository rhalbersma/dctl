#pragma once
#include <dctl/rule_traits.hpp> // precedence::is_trivial, precedence::greater, large_jump_v
#include <algorithm>            // max_element, stable_sort, unique, upper_bound
#include <cassert>
#include <iterator>             // begin, end, distance

namespace dctl {
namespace actions {
namespace filter {

template<class Rules>
class Precedence
{
public:
        template<class RandomAccessIterator>
        auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
        {
                if (precedence::is_trivial_v<Rules> || std::distance(first, last) < 2)
                        return last;

                std::stable_sort(first, last, precedence::greater_t<Rules>{});
                return std::upper_bound(first, last, *first, precedence::greater_t<Rules>{});
        }
};

template<class Rules>
class Unique
{
public:
        template<class RandomAccessIterator>
        auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
        {
                if (std::distance(first, last) < 2)
                        return last;

                auto const smaller = [](auto const& lhs, auto const& rhs){
                        return lhs.num_captured() < rhs.num_captured();
                };

                auto const m = std::max_element(first, last, smaller);
                assert(m != last);
                if (m->num_captured() < large_jump_v<Rules>)
                        return last;

                std::stable_sort(first, last);
                return std::unique(first, last);
        }
};

}       // namespace filter
}       // namespace actions
}       // namespace dctl