#pragma once
#include <dctl/rule_traits.hpp> // equal_to, less, equivalent_to
#include <range/v3/all.hpp>     // adjacent_find, is_sorted

namespace dctl {
namespace precedence {

template<class Rules, class Range>
auto is_consistent(Range const& rng)
{
        return
                (ranges::adjacent_find(rng, equal_to_t     <Rules>{}) == cend(rng)) &&
                (ranges::adjacent_find(rng, equivalent_to_t<Rules>{}) == cend(rng)) &&
                 ranges::is_sorted    (rng, less_t         <Rules>{})
        ;
}

}       // namespace precedence
}       // namespace dctl
