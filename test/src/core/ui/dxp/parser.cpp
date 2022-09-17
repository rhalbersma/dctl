//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/ui/dxp/parser.hpp>
#include <dctl/core/ui/dxp/message.hpp> // factory
//#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <stdexcept>                    // invalid_argument
#include <string>                       // string
#include <vector>                       // vector
#include <variant>                      // visit

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(DXPParser)

        BOOST_FIXTURE_TEST_CASE(MesanderMessageExamples, Fixture)
        {
                // Examples of DXP messages (Layer 2 protocol description)
                // http://www.mesander.nl/damexchange/edxplg2.htm

                std::vector<std::string> messages =
                {
                        "R01Tornado voor Windows 4.0        W060065A",
                        "ATornado voor Windows 4.0        0",
                        "M0012061100",
                        "M001205250422122320",
                        "E00",
                        "CWhat do you think about move 35?",
                        "B005Z",
                        "K1"
                };

                for (auto const& m : messages) {
                        auto value = parse(m);
                        BOOST_CHECK_EQUAL(std::visit([](auto const& v){ return v.str(); }, value), m);
                }
                BOOST_CHECK_THROW(parse("This is not a valid DXP message"), std::invalid_argument);
/*
                auto const initial = basic_state<international, basic_board<international>>::initial();
                auto gamereq = parse(messages[0]);
                BOOST_CHECK_EQUAL(std::visit(to_state, gamereq), initial);
                for (auto it = std::next(messages.begin()); it != messages.end(); ++it) {
                        auto value = parse(*it);
                        BOOST_CHECK_THROW(std::visit(to_state, value), std::invalid_argument);
                }
*/
        }

BOOST_AUTO_TEST_SUITE_END()
