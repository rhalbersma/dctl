#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_GameAcknowledge: public DXP_AbstractMessage
{
public:
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        DXP_GameAcknowledge(const std::string&);
        DXP_GameAcknowledge(const std::string&, AcceptanceCode);

        std::string send_message(void) const;

        static const char HEADER = 'A';

private:    
        std::string name_follower;
        AcceptanceCode acceptance_code;
};
