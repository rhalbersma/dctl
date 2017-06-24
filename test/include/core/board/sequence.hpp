#pragma once

//          Copyright Rein Halbersma 2010-2017.
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
        pack<10, 9>,
        pack<11, 9>,
        international,
        frisian,
        ktar<10, 11>,
        ktar<10, 12>,
        pack<12, 10>,
        pack<12, 12>,
        pack<12, 12, true>,
        pack<14, 14>,
        pack<19, 10>
>;

template<class T>
struct wrap_rectangular
{
    using type = rectangular<T>;
};

using BoardSequence = typename boost::mpl::transform<
        Sequence,
        wrap_rectangular<boost::mpl::_1>
>::type;

}       // namespace dctl::core
