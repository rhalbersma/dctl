#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/container.hpp>

namespace dctl::algo {
namespace search {

using Order = dctl::core::model::default_container<int>;
using Variation = dctl::core::model::default_container<int>;

inline void update_variation(Variation& line, int move, Variation const& continuation)
{
        line.clear();
        line.push_back(move);
        line.insert(line.begin(), continuation.cbegin(), continuation.cend());
}

}       // namespace search
}       // namespace dctl::algo
