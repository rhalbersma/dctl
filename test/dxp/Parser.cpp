#include <string>                       // string
#include <boost/mpl/vector.hpp>         // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_CHECK_EQUAL
#include <dctl/dxp/IMessage.hpp>        // IMessage
#include <dctl/dxp/Types.hpp>           // GameRequest, GameAcknowledge, Move, GameEnd, Chat, BackRequest, BackAcknowledge
#include <dctl/factory/Factory.hpp>     // Factory

namespace dctl {
namespace dxp {

BOOST_AUTO_TEST_SUITE(TestParser)

BOOST_AUTO_TEST_CASE(MesanderExamples)
{
        // Examples of DXP messages (Layer 2 protocol description)
        // http://www.mesander.nl/damexchange/edxplg2.htm
        std::string const messages[] = {
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
                IMessage
        > factory;

        for (auto const& m: messages) {
                if (auto const parsed = factory.create(m))
                        BOOST_CHECK_EQUAL(m, parsed->str());
                else
                        BOOST_CHECK(!"Factory cannot create an unregistered message type.");
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
