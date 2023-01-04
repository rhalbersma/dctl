#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>    // adjacent_find
#include <functional>   // greater_equal

namespace dctl::core {

constexpr auto is_strictly_sorted(auto const& rng) noexcept
{
        return std::ranges::adjacent_find(rng, std::greater_equal()) == rng.end();
}

}       // namespace dctl::core
