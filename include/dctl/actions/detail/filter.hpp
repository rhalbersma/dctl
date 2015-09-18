#pragma once
#include <dctl/rule_traits.hpp> // precedence::is_trivial, precedence::less, precedence::equal_to, large_jump_v
#include <algorithm>            // max_element, find_if_not, copy_if, stable_sort, unique
#include <iterator>             // begin, end, distance, make_move_iterator

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

                auto it = std::max_element(first, last, precedence::less_t<Rules>{});
                auto const dst = std::find_if_not(first, last, [&](auto const& move){
                        return precedence::equal_to_t<Rules>{}(move, *it);
                });
                auto const src = std::next(it, std::distance(first, dst));
                return std::copy_if(std::make_move_iterator(src), std::make_move_iterator(last), dst, [&](auto const& move){
                        return precedence::equal_to_t<Rules>{}(move, *it);
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
