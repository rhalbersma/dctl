//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>    // color, black, white, black_, white_, black_c, white_c, not_
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE
#include <concepts>                     // same_as

BOOST_AUTO_TEST_SUITE(Color)

using namespace dctl::core;

BOOST_AUTO_TEST_CASE(ColorsAreDifferent)
{
        static_assert(color::black != color::white);
        static_assert(color::white != color::black);

        static_assert(!std::same_as<black_, white_>);
        static_assert(!std::same_as<white_, black_>);

        static_assert(black_c != white_c);
        static_assert(white_c != black_c);
}

BOOST_AUTO_TEST_CASE(ColorsAreNegativelyRelated)
{
        static_assert((!color::black) == color::white);
        static_assert((!color::white) == color::black);

        static_assert(std::same_as<not_<black_>, white_>);
        static_assert(std::same_as<not_<white_>, black_>);

        static_assert((!black_c) == white_c);
        static_assert((!white_c) == black_c);
}

BOOST_AUTO_TEST_SUITE_END()
