#include <iterator>                             // begin, end
#include <string>                               // string
#include <boost/test/unit_test.hpp>             // BOOST_CHECK_EQUAL
#include <boost/mpl/vector.hpp>                 // vector
#include "../../src/dxp/MessageInterface.hpp"
#include "../../src/dxp/Types.hpp"
#include "../../src/factory/Factory.hpp"

namespace dctl {
namespace dxp {

BOOST_AUTO_TEST_SUITE(TestParser)

BOOST_AUTO_TEST_CASE(MesanderExamples)
{
        // Examples of DXP messages (Layer 2 protocol description)
        // http://www.mesander.nl/damexchange/edxplg2.htm
        std::string const messages[] =
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

        typedef boost::mpl::vector<
                GameRequest, GameAcknowledge, Move, GameEnd, Chat, BackRequest, BackAcknowledge
        > MessageTypes;

        Factory<
                MessageTypes,
                MessageInterface
        > factory;

        for (auto it = std::begin(messages); it != std::end(messages); ++it) {
                if (auto const parsed = factory.create(*it))
                        BOOST_CHECK_EQUAL(*it, parsed->str());
                else
                        BOOST_CHECK(!"Factory cannot create an unregistered message type.");
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
