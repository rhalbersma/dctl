#include "DXP_Test.h"
#include <cassert>
#include <iostream>

// Examples of DamExchange messages (Layer 2 Protocol): http://www.mesander.nl/damexchange/edxplg2.htm
void DXP_Test::Mesander_examples(void)
{
        std::string message[] = {
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
                const DXP_AbstractMessage* m = DXP_MessageFactory::create(message[i]);
                assert(!message[i].compare(m->message()));
                std::cout << m->message() << std::endl;
                delete m;
        }
}
