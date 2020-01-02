#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // equal_to, less, equivalent_to
#include <algorithm>                            // adjacent_find, is_sorted

namespace dctl::core {
namespace xxx_precedence {

template<class Range>
auto is_consistent(Range const& rng)
{
        return
                (std::adjacent_find(rng.begin(), rng.end(), equal_to     {}) == rng.end()) &&
                (std::adjacent_find(rng.begin(), rng.end(), equivalent_to{}) == rng.end()) &&
                 std::is_sorted    (rng.begin(), rng.end(), less         {})
        ;
}

}       // namespace xxx_precedence
}       // namespace dctl::core
