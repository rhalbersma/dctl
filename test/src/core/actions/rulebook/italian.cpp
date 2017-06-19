
//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/actions/fixture.hpp>          // run
#include <dctl/core/board.hpp>               // Roman
#include <dctl/core/rules.hpp>               // italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

using namespace dctl::core;

// States from the official italian rules:
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

BOOST_AUTO_TEST_SUITE(ActionsRulebookitalian)

using F = Fixture<italian, board::italian>;

BOOST_FIXTURE_TEST_CASE(pawn_jumpDirections, F)          // Art. 5.6
{
        auto const fen = "W:W30:B27";
        auto const legal = std::vector<std::string>{ "30x23" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(pawn_jumpContinuation, F)        // Art. 5.7
{
        auto const fen = "W:W31:B12,20,28";
        auto const legal = std::vector<std::string>{ "31x08" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(king_jumpDirection, F)           // Art. 5.8
{
        auto const fen = "W:WK22:B18,19,27";
        auto const legal = std::vector<std::string>{ "22x13", "22x15", "22x31" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(king_jumpContinuation, F)        // Art. 5.9
{
        auto const fen = "W:WK27:B5,6,13,15,21,23";
        auto const legal = std::vector<std::string>{ "27x25" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(NumberOfpieces, F)              // Art. 6.6
{
        auto const fen = "W:WK21,31:B10,18,27";
        auto const legal = std::vector<std::string>{ "31x06" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(WithKing, F)                    // Art. 6.7
{
        auto const fen = "W:WK23,31:B19,26,27";
        auto const legal = std::vector<std::string>{ "23x21" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(NumberOfKings, F)               // Art. 6.8
{
        auto const fen = "W:WK23:B19,K28";
        auto const legal = std::vector<std::string>{ "23x32" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(OrderOfKings, F)                // Art. 6.9
{
        auto const fen = "W:WK30:B10,12,18,K20,K26,27";
        auto const legal = std::vector<std::string>{ "30x05" };
        test(fen, legal);
}

BOOST_FIXTURE_TEST_CASE(Equivalent, F)                  // Art. 6.10
{
        auto const fen = "W:WK31:B18,20,K27,K28";
        auto const legal = std::vector<std::string>{ "31x13", "31x15" };
        test(fen, legal);
}

BOOST_AUTO_TEST_SUITE_END()
