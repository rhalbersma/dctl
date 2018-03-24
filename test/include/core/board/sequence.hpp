#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>
#include <dctl/core/rules.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl::core {

using Sequence = boost::mpl::vector
<
        nano,
        micro,
        checkers,
        italian,
        spantsiretti,
        rectangular<10, 9>,
        rectangular<11, 9>,
        international,
        frisian,
        ktar<10, 11>,
        ktar<10, 12>,
        rectangular<12, 10>,
        rectangular<12, 12>,
        rectangular<12, 12, true>,
        rectangular<14, 14>,
        rectangular<19, 10>
>;

template<class T>
struct wrap_board
{
    using type = basic_board<T>;
};

using BoardSequence = typename boost::mpl::transform<
        Sequence,
        wrap_board<boost::mpl::_1>
>::type;

}       // namespace dctl::core
