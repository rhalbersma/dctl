//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/ui/dxp/parser.hpp>
#include <dctl/core/ui/dxp/message.hpp> // parser
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/variant.hpp>            // apply_visitor
#include <string>                       // string
#include <vector>                       // vector
#include <iostream>

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
                        BOOST_CHECK_EQUAL(boost::apply_visitor(to_string, value), m);
                }
                BOOST_CHECK_THROW(parse("This is not a valid DXP message"), std::invalid_argument);

                auto const state = basic_state<international, board<international>>::initial();
                auto gamereq = parse(messages[0]);
                BOOST_CHECK_EQUAL(boost::apply_visitor(state_visitor, gamereq), state);
        }

BOOST_AUTO_TEST_SUITE_END()
