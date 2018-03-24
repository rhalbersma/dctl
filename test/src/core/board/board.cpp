//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>              // nano, micro, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, basic_board<12, 10>, canadian, srilankan, dumm
#include <boost/range/irange.hpp>               // irange
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                            // all_of

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(Bit2SquareIsInverseSquare2Bit, T, BoardSequence)
{
        auto const squares = boost::irange(0, T::size());
        BOOST_CHECK(
                std::all_of(squares.begin(), squares.end(), [](auto const sq) {
                        return T::square_from_bit(T::bit_from_square(sq)) == sq;
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
