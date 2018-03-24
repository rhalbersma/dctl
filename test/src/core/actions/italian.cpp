//          Copyright Rein Halbersma 2010-2018.
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

BOOST_AUTO_TEST_SUITE(ActionsItalian)

        using F = Fixture<italian>;

        BOOST_AUTO_TEST_SUITE(ItalianOfficialVersion)

                // Positions from the official Italian rules:
                // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

                BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)          // Art. 5.6
                {
                        auto const fen = "W:W30:B27";
                        auto const legal = std::vector<std::string>{ "30x23" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)        // Art. 5.7
                {
                        auto const fen = "W:W31:B12,20,28";
                        auto const legal = std::vector<std::string>{ "31x08" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpDirection, F)           // Art. 5.8
                {
                        auto const fen = "W:WK22:B18,19,27";
                        auto const legal = std::vector<std::string>{ "22x13", "22x15", "22x31" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)        // Art. 5.9
                {
                        auto const fen = "W:WK27:B5,6,13,15,21,23";
                        auto const legal = std::vector<std::string>{ "27x25" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(QuantityPrecedence, F)          // Art. 6.6
                {
                        auto const fen = "W:WK21,31:B10,18,27";
                        auto const legal = std::vector<std::string>{ "31x06" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(ModalityPrecedence, F)          // Art. 6.7
                {
                        auto const fen = "W:WK23,31:B19,26,27";
                        auto const legal = std::vector<std::string>{ "23x21" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(ContentsPrecedence, F)          // Art. 6.8
                {
                        auto const fen = "W:WK23:B19,K28";
                        auto const legal = std::vector<std::string>{ "23x32" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(OrderingPrecedence, F)          // Art. 6.9
                {
                        auto const fen = "W:WK30:B10,12,18,K20,K26,27";
                        auto const legal = std::vector<std::string>{ "30x05" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(EquivalentJumps, F)             // Art. 6.10
                {
                        auto const fen = "W:WK31:B18,20,K27,K28";
                        auto const legal = std::vector<std::string>{ "31x13", "31x15" };
                        test(fen, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(AliotoDomenico)

                // Suggested by Alioto Domenico in email to Ed Gilbert

                BOOST_FIXTURE_TEST_CASE(OrderingPrecedence, F)          // Art. 6.9
                {
                        auto const fen = "W:WK30:B10,K12,K18,20,K26,K27";
                        auto const legal = std::vector<std::string>{ "30x05" };
                        test(fen, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(JumpModality)

                BOOST_FIXTURE_TEST_CASE(PawnsOnlyJumpForward, F)        // Art 5.3(b)
                {
                        auto const fen = "W:W24:B20,28";
                        auto const legal = std::vector<std::string>{ "24x15" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnsCannotJumpKings, F)        // Art 5.3(b)
                {
                        auto const fen = std::vector<std::string>{ "W:W30:BK26", "W:W30:B27" };
                        auto const legal = std::vector<std::vector<std::string>>{ { "30-27" }, { "30x23" } };
                        for (auto i = std::size_t{0}; i < fen.size(); ++i) {
                                test(fen[i], legal[i]);
                        }
                }

                BOOST_FIXTURE_TEST_CASE(KingsJumpForwardAndBackward, F) // Art 5.3(c)
                {
                        auto const fen = "W:WK24:B20,28";
                        auto const legal = std::vector<std::string>{ "24x15", "24x31" };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingsJumpPawnsAndKings, F)      // Art 5.3(d)
                {
                        auto const fen = std::vector<std::string>{ "W:WK30:BK26", "W:WK30:B27" };
                        auto const legal = std::vector<std::vector<std::string>>{ { "30x21" }, { "30x23" } };
                        for (auto i = std::size_t{0}; i < fen.size(); ++i) {
                                test(fen[i], legal[i]);
                        }
                }

        BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
