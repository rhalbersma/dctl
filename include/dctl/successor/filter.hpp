#pragma once
#include <dctl/rule_traits.hpp> // jump_precedence_t, is_jump_precedence_v, large_jump_v
#include <algorithm>            // max_element, stable_sort, unique, upper_bound
#include <iterator>             // begin, end, distance

namespace dctl {
namespace successor {
namespace filter {

template<class Rules>
class Precedence
{
public:
        template<class RandomAccessIterator>
        auto operator()(RandomAccessIterator first, RandomAccessIterator last) const
        {
                if (!is_jump_precedence_v<Rules> || std::distance(first, last) < 2)
                        return last;

                auto const greater = [](auto const& L, auto const& R){
                        return jump_precedence_t<Rules>{}(R, L);
                };

                std::stable_sort(first, last, greater);
                return std::upper_bound(first, last, *first, greater);
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

                auto const smaller = [](auto const& L, auto const& R){
                        return L.num_pieces() < R.num_pieces();
                };

                auto const m = std::max_element(first, last, smaller);
                if (m->num_pieces() < large_jump_v<Rules>)
                        return last;

                std::stable_sort(first, last);
                return std::unique(first, last);
        }
};

}       // namespace filter
}       // namespace successor
}       // namespace dctl
