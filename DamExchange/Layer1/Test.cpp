#include "Test.h"
#include "Client.h"
#include <iostream>
#include <string>

namespace DXP = DamExchange;

void DXP::Layer1::Test::connection(void)
{
        Client dxp_connection;
        // only one of these next two statements can be active!!
        //dxp_connection.connect("127.0.0.1", "27531");
        dxp_connection.accept("27531");

        std::string line;
        while (std::getline(std::cin, line))
        {                
                dxp_connection.write(line);
        }
        dxp_connection.close();
}
