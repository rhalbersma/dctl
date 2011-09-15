#include <string>                       // std::string
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL 
#include "../../src/dxp/Types.hpp"
#include "../../src/dxp/Parser.hpp"
#include "../../src/dxp/Protocol.hpp"
#include "../../src/dxp/Connection.hpp"

namespace dctl {
namespace dxp {

BOOST_AUTO_TEST_SUITE(TestParser)

// Examples of DXP messages (Layer 2 Protocol)
// http://www.mesander.nl/dxp/edxplg2.htm
BOOST_AUTO_TEST_CASE(MesanderExamples)
{
        // the element type of an array cannot be a type that contains 'auto'
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

        for (auto i = 0; i < 8; ++i) {                
                if (auto parsed = Parser::create_message(message[i]))
                        // the message type did get registered, compare the original and the parsed message
                        BOOST_CHECK_EQUAL(message[i], parsed->str());
                else {
                        // the message type did not get registered, do not dereference a nullptr
                        BOOST_CHECK(false);
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
