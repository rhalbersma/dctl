#pragma once
#include <dctl/rule_traits.hpp> // equal_to, less, equivalent_to
#include <algorithm>            // adjacent_find, is_sorted

namespace dctl {
namespace precedence {

template<class Rules, class Range>
auto is_consistent(Range const& rng)
{
        return
                (std::adjacent_find(rng.cbegin(), rng.cend(), equal_to     <Rules>{}) == rng.cend()) &&
                (std::adjacent_find(rng.cbegin(), rng.cend(), equivalent_to<Rules>{}) == rng.cend()) &&
                 std::is_sorted    (rng.cbegin(), rng.cend(), less         <Rules>{})
        ;
}

}       // namespace precedence
}       // namespace dctl
