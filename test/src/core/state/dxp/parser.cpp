#include <dctl/core/state/dxp.hpp>      // game_request, game_acknowledge, move, back_request, back_acknowledge, game_end, chat, factory
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/variant.hpp>            // apply_visitor
#include <string>                       // string
#include <variant>                      // monostate
#include <vector>                       // vector
#include <core/state/dxp/parser.hpp>

using namespace dctl::core;
using F = dxp::Fixture;

BOOST_AUTO_TEST_SUITE(DXPParser)

	BOOST_FIXTURE_TEST_CASE(MesanderMessageExamples, F)
	{
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


		for (auto const& m : messages) {
		        auto v = f.create(m);
		        boost::apply_visitor(vis, v);
		}

		BOOST_CHECK_EQUAL_COLLECTIONS(messages.begin(), messages.end(), parsed.begin(), parsed.end());
	}

	BOOST_FIXTURE_TEST_CASE(InvalidMessagesAreIgnored, F)
	{
		auto const n = vis.data.size();
		auto v = f.create("This is an invalid message");
		boost::apply_visitor(vis, v);
		BOOST_CHECK_EQUAL(vis.data.size(), n);
	}

BOOST_AUTO_TEST_SUITE_END()
