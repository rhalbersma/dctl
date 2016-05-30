#pragma once
#include <dctl/rule_traits.hpp> // equal_to, less, equivalent_to
#include <algorithm>            // adjacent_find, is_sorted

namespace dctl {
namespace precedence {

template<class Range>
auto is_consistent(Range const& rng)
{
        return
                (std::adjacent_find(rng.cbegin(), rng.cend(), equal_to     {}) == rng.cend()) &&
                (std::adjacent_find(rng.cbegin(), rng.cend(), equivalent_to{}) == rng.cend()) &&
                 std::is_sorted    (rng.cbegin(), rng.cend(), less         {})
        ;
}

}       // namespace precedence
}       // namespace dctl
