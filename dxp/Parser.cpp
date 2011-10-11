#include <string>                               // std::string
#include <boost/mpl/list.hpp>                   // boost::mpl::list
#include <boost/test/unit_test.hpp>             // BOOST_CHECK_EQUAL 
//#include "../../src/dxp/Connection.hpp"
#include "../../src/dxp/MessageInterface.hpp"
#include "../../src/dxp/Parser.hpp"
#include "../../src/dxp/Types.hpp"

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

        Parser<
                boost::mpl::list<
                        GameRequest, GameAcknowledge, Move, GameEnd, Chat, BackRequest, BackAcknowledge
                >,
                MessageInterface
        > p;

        for (auto i = 0; i < 8; ++i) {                
                if (auto parsed = p.parse(message[i]))
                        // if the message type did get registered, compare the original and the parsed message
                        BOOST_CHECK_EQUAL(message[i], parsed->str());
                else {
                        // if the message type did not get registered, do not dereference a nullptr
                        BOOST_CHECK(false);
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
