#include "Test.h"
#include "Session.h"
#include <iostream>
#include <string>

namespace DXP = DamExchange;

void DXP::Layer1::Test::session(void)
{
        Session dxp_session;
        // only one of these next two statements can be active!!
        dxp_session.connect();
        
        //dxp_session.accept();

        std::string line;
        while (std::getline(std::cin, line))
        {                
                dxp_session.write(line);
        }
        dxp_session.close();
}
