#include <dctl/core/state/ui/dxp.hpp>   // game_request, game_acknowledge, move, back_request, back_acknowledge, game_end, chat, factory
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(DXPParser)

BOOST_AUTO_TEST_CASE(MesanderMessageExamples)
{
        factory
        <
                dxp::game_request,
                dxp::game_acknowledge,
                dxp::move,
                dxp::back_request,
                dxp::back_acknowledge,
                dxp::game_end,
                dxp::chat
        > f;

        // Examples of DXP messages (Layer 2 protocol description)
        // http://www.mesander.nl/damexchange/edxplg2.htm
        std::vector<std::string> messages =
        {
                "R01Tornado voor Windows 4.0        W060065A",
                "ATornado voor Windows 4.0        0",
                "M0012061100",
                "M001205250422122320",
                "B005Z",
                "K1",
                "E00",
                "CWhat do you think about move 35?"
        };

        std::vector<std::string> parsed;
        for (auto const& m : messages) {
                f.visit(f.create(m), [&](auto const& v){ parsed.push_back(v.str()); });
        }

        BOOST_CHECK_EQUAL_COLLECTIONS(begin(messages), end(messages), begin(parsed), end(parsed));
}

BOOST_AUTO_TEST_SUITE_END()
