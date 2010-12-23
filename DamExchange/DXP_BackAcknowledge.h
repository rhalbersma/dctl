#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_BackAcknowledge: public DXP_AbstractMessage
{
public:
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        DXP_BackAcknowledge(const std::string&);
        DXP_BackAcknowledge(AcceptanceCode);

        std::string send_message(void) const;

        static const char HEADER = 'K';

private:
        AcceptanceCode acceptance_code;
};
