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
        const std::string message[] =
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
        > Messages;

        Factory<
                Messages,
                MessageInterface
        > factory;

        for (auto i = 0; i < 8; ++i) {
                if (const auto parsed = factory.create(message[i]))
                        BOOST_CHECK_EQUAL(message[i], parsed->str());
                else
                        BOOST_CHECK(!"Unregistered message type (cannot dereference a nullptr)");
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
