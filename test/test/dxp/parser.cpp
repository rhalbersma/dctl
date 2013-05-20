#include <algorithm>                    // transform
#include <iterator>                     // back_inserter
#include <string>                       // string
#include <vector>                       // vector
#include <boost/mpl/vector.hpp>         // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_CHECK_EQUAL
#include <dctl/dxp/i_message.hpp>       // IMessage
#include <dctl/dxp/types.hpp>           // GameRequest, GameAcknowledge, Move, GameEnd, Chat, BackRequest, BackAcknowledge
#include <dctl/factory/factory.hpp>     // Factory

namespace dctl {
namespace dxp {

BOOST_AUTO_TEST_SUITE(TestParser)

BOOST_AUTO_TEST_CASE(MesanderExamples)
{
        typedef boost::mpl::vector<
                GameRequest, GameAcknowledge, Move, GameEnd, Chat, BackRequest, BackAcknowledge
        > MessageTypes;

        Factory<MessageTypes, IMessage> factory;

        // Examples of DXP messages (Layer 2 protocol description)
        // http://www.mesander.nl/damexchange/edxplg2.htm
        std::vector<std::string> messages = {
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
        parsed.reserve(messages.size());
        std::transform(begin(messages), end(messages), std::back_inserter(parsed),
                [&](std::string const& m) {
                auto const p = factory.create(m);
                return p->str();
        });

        BOOST_CHECK_EQUAL_COLLECTIONS(begin(messages), end(messages), begin(parsed), end(parsed));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
