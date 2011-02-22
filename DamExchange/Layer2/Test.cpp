#include "Test.h"
#include "Parser.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

namespace DXP = DamExchange;

// Examples of DamExchange messages (Layer 2 Protocol): http://www.mesander.nl/damexchange/edxplg2.htm
void DXP::Layer2::Test::Mesander_examples(void)
{
        const std::string example[] = {
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
                std::shared_ptr<AbstractMessage> msg = Parser::find(example[i]);
                assert(!example[i].compare(msg->str()));
                std::cout << msg->str() << std::endl;
        }
}
