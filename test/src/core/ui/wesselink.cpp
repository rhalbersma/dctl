//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>          // international
#include <dctl/core/rules.hpp>          // international
#include <dctl/core/ui/dxp.hpp>         // read
#include <dctl/core/ui/pdn.hpp>         // read, write
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
        // with a modified character set (default is basic_token_set<'Z', 'W', 'e'>)
        auto const pos_w = dxp::read<international, basic_board<international>, basic_token_set<'X', 'O', '.'>>{}(w);

        // write the above position as a fen string with the PDN protocol
        auto const fen_w = pdn::write<>{}(pos_w);

        // convert the new fen string to a new position and back into a new fen string
        auto const pos_r = pdn::read<international>{}(fen_w);
        auto const fen_r = pdn::write<>{}(pos_r);

        // the two positions have been parsed through different protocols,
        // but they should have identical fen strings
        BOOST_CHECK_EQUAL(fen_w, fen_r);
}

BOOST_AUTO_TEST_SUITE_END()
