#include "Test.h"
#include "StringMessage.h"
#include <cassert>
#include <iostream>
#include <memory>

namespace DXP = DamExchangeProtocol;

// Examples of DamExchange messages (Layer 2 Protocol): http://www.mesander.nl/damexchange/edxplg2.htm
void DXP::Test::Mesander_examples(void)
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
                const DXP::StringMessage s(example[i]);
                std::shared_ptr<DXP::AbstractMessage> m = DXP::MessageFactory::select_creator(s);
                assert(!s.str().compare(m->message().str()));
                std::cout << m->message().str() << std::endl;
        }
}
