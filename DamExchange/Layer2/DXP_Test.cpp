#include "DXP_Test.h"
#include "DXP_String.h"
#include <cassert>
#include <iostream>

// Examples of DamExchange messages (Layer 2 Protocol): http://www.mesander.nl/damexchange/edxplg2.htm
void DXP_Test::Mesander_examples(void)
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
                const DXP_String s(example[i]);
                const DXP_AbstractMessage* m = DXP_MessageFactory::select_creator(s);
                assert(!s.str().compare(m->message().str()));
                std::cout << m->message().str() << std::endl;
                delete m;
        }
}
