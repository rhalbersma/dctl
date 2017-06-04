#include <dctl/core/state/ui/dxp/message.hpp>                      // Message
#include <dctl/core/state/ui/dxp/types.hpp>                        // GameRequest, GameAcknowledge, Action, GameEnd, Chat, BackRequest, BackAcknowledge
#include <dctl/util/factory/factory.hpp>             // Factory
#include <boost/mpl/for_each.hpp>                       // for_each
#include <boost/mpl/identity.hpp>                       // make_identity
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/range/adaptor/transformed.hpp>          // transformed
#include <boost/range/algorithm_ext/push_back.hpp>      // push_back
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <string>                                       // string
#include <vector>                                       // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(DXPParser)

using Messages = boost::mpl::vector
<
        dxp::GameRequest, dxp::GameAcknowledge, dxp::Move, dxp::GameEnd, dxp::Chat, dxp::BackRequest, dxp::BackAcknowledge
>;

BOOST_AUTO_TEST_CASE(MesanderMessageExamples)
{
        Factory<dxp::Message> f;
        boost::mpl::for_each<Messages, boost::mpl::make_identity<>>([&](auto Id) {
                using T = typename decltype(Id)::type;
                f.insert(T::identifier(), T::create);
        });

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

        std::vector<std::string> parsed;
        using boost::adaptors::transformed;
        boost::push_back(parsed, messages | transformed([&](auto const& m) {
                auto const p = f.create(m);
                return p->str();
        }));

        BOOST_CHECK_EQUAL_COLLECTIONS(begin(messages), end(messages), begin(parsed), end(parsed));
}

BOOST_AUTO_TEST_SUITE_END()
