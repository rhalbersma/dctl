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
        template<class BiDirectionalIterator>
        auto operator()(BiDirectionalIterator first, BiDirectionalIterator last) const
        {
                if (precedence::is_trivial_v<Rules> || std::distance(first, last) < 2)
                        return last;

                auto const max_precedence = *std::max_element(first, last, precedence::less_t<Rules>{});
                return std::stable_partition(first, last, [&](auto const& move){
                        return precedence::equal_to_t<Rules>{}(move, max_precedence);
                });
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

                auto const max_num_captured = std::max_element(first, last, [](auto const& lhs, auto const& rhs){
                        return lhs.num_captured() < rhs.num_captured();
                })->num_captured();
                if (max_num_captured < large_jump_v<Rules>)
                        return last;

                std::stable_sort(first, last);
                return std::unique(first, last);
        }
};

}       // namespace filter
}       // namespace actions
}       // namespace dctl
