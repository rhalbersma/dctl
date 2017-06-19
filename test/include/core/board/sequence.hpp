#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl::core {
namespace board {

using BoardSequence = boost::mpl::vector
<
        micro,
        mini,
        checkers,
        italian,
        spantsiretti,
        international,
        frisian,
        ktar<10, 11>,
        ktar<10, 12>,
        rectangular<12, 10>,
        canadian,
        srilankan,
        dumm
>;

}       // namespace board
}       // namespace dctl::core
