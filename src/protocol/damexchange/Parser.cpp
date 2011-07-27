#include <string>
#include "gtest/gtest.h"
#include "../../../../Library/src/protocol/damexchange/Parser.h"
#include "../../../../Library/src/protocol/damexchange/Version.h"

namespace dctl {
namespace protocol {
namespace damexchange {

// Examples of DamExchange messages (Layer 2 Protocol)
// http://www.mesander.nl/damexchange/edxplg2.htm
TEST(Parser, MesanderExamples)
{
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
                auto parsed = Parser<version>::find(message[i]);
                EXPECT_EQ(0, parsed->str().compare(message[i]));
        }
}

}       // namespace damexchange
}       // namespace protocol
}       // namespace dctl
