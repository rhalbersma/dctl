//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>          // international
#include <dctl/core/rules.hpp>          // international
#include <dctl/core/state.hpp>          // read, write
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(SetupWesselink)

BOOST_AUTO_TEST_CASE(WiegerWesselink)
{
        // Test position from email conversation with Wieger Wesselink
        auto const w = "O     \
            .   .   .   .   . \
          .   .   .   .   .   \
            .   .   x   x   x \
          x   .   .   x   x   \
            x   .   .   x   x \
          .   o   o   o   .   \
            .   o   o   o   o \
          .   o   .   o   o   \
            .   .   .   o   o \
          .   .   .   .   .   \
        ";

        // parse the above diagram into a position with the DamExchange protocol
        // with a modified character set (default is <'Z', 'W', 'E'>)
        auto const pos_w = setup::read<
                international, rectangular<international>, dxp::protocol, detail::token_interface<'X', 'O', '.'>
        >()(w);

        // write the above position as a fen string with the PDN protocol
        auto const fen_w = setup::write<pdn::protocol>()(pos_w);

        // convert the new fen string to a new position and back into a new fen string
        auto const pos_r = setup::read<international>()(fen_w);
        auto const fen_r = setup::write<pdn::protocol>()(pos_r);

        // the two positions have been parsed through different protocols,
        // but they should have identical fen strings
        BOOST_CHECK_EQUAL(fen_w, fen_r);
}

BOOST_AUTO_TEST_SUITE_END()
