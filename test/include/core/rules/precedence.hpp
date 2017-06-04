#pragma once
#include <dctl/core/rules/traits.hpp>                 // equal_to, less, equivalent_to
#include <boost/range/algorithm.hpp>            // adjacent_find
#include <boost/range/algorithm_ext.hpp>        // is_sorted

namespace dctl {
namespace core {
namespace precedence {

template<class Range>
auto is_consistent(Range const& rng)
{
        return
                (boost::adjacent_find(rng, equal_to     {}) == rng.cend()) &&
                (boost::adjacent_find(rng, equivalent_to{}) == rng.cend()) &&
                 boost::is_sorted    (rng, less         {})
        ;
}

}       // namespace precedence
}       // namespace core
}       // namespace dctl
