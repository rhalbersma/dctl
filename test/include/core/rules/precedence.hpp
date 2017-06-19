#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>                 // equal_to, less, equivalent_to
#include <boost/range/algorithm.hpp>            // adjacent_find
#include <boost/range/algorithm_ext.hpp>        // is_sorted

namespace dctl::core {
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
}       // namespace dctl::core
