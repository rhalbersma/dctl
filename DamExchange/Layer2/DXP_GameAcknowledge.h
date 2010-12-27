#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_GameAcknowledge: public DXP_AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        // constructors
        DXP_GameAcknowledge(const std::string&);
        DXP_GameAcknowledge(const std::string&, AcceptanceCode);

        // views
        std::string message(void) const;
        const std::string& name_follower(void) const;
        AcceptanceCode acceptance_code(void) const;

        // implementation
        static const char HEADER = 'A';

private:    
        // representation
        std::string d_name_follower;
        AcceptanceCode d_acceptance_code;
};
