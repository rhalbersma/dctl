#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_BackAcknowledge: public DXP_AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // constructors
        DXP_BackAcknowledge(const std::string&);
        DXP_BackAcknowledge(AcceptanceCode);

        // views
        std::string message(void) const;
        AcceptanceCode acceptance_code(void) const;

        // implementation
        static const char HEADER = 'K';

private:
        // representation
        AcceptanceCode d_acceptance_code;
};
