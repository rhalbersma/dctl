#include <dctl/core/state/dxp.hpp>      // game_request, game_acknowledge, move, back_request, back_acknowledge, game_end, chat, factory
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/variant.hpp>            // apply_visitor
#include <string>                       // string
#include <variant>                      // monostate
#include <vector>                       // vector

using namespace dctl::core;

struct visitor
{
        std::vector<std::string>& vec;

        template<class T>
        auto operator()(T const& value) { vec.push_back(value.str()); }

        auto operator()(std::monostate) {}
};

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
        auto vis = visitor{parsed};

        for (auto const& m : messages) {
                auto v = f.create(m);
                boost::apply_visitor(vis, v);
        }

        BOOST_CHECK_EQUAL_COLLECTIONS(messages.begin(), messages.end(), parsed.begin(), parsed.end());

	BOOST_CHECK_EQUAL(f.create("This is an invalid message").which(), 0);

}

BOOST_AUTO_TEST_SUITE_END()
