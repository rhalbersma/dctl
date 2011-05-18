#include "../../../../Library/Source/DamExchange/Layer2/Parser.h"

#include "gtest/gtest.h"
#include <memory>
#include <string>

namespace DXP = DamExchange;

// Examples of DamExchange messages (Layer 2 Protocol): http://www.mesander.nl/damexchange/edxplg2.htm
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

        for (size_t i = 0; i < 8; ++i) {
                std::shared_ptr<DXP::Layer2::AbstractMessage> parsed = DXP::Layer2::Parser::find(message[i]);
                EXPECT_EQ(0, parsed->str().compare(message[i]));
        }
}
