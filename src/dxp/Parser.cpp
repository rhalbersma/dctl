#include <string>
#include <boost/test/unit_test.hpp> 
#include "../../../DCTL/src/dxp/Parser.h"
#include "../../../DCTL/src/dxp/DXP.h"

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
                auto parsed = Parser<protocol>::create_message(message[i]);
                BOOST_CHECK_EQUAL(0, parsed->str().compare(message[i]));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dxp
}       // namespace dctl
