#pragma once
#include <dctl/rule_traits.hpp> // precedence: equal_to, less, equivalent_to
#include <algorithm>            // adjacent_find, sort

namespace dctl {
namespace precedence {

template<class Rules, class ForwardIterator>
auto is_consistent(ForwardIterator first, ForwardIterator last)
{
        return
                (std::adjacent_find(first, last, equal_to_t     <Rules>{}) == last) &&
                (std::adjacent_find(first, last, equivalent_to_t<Rules>{}) == last) &&
                 std::is_sorted    (first, last, less_t         <Rules>{})
        ;
}

}       // namespace precedence
}       // namespace dctl
