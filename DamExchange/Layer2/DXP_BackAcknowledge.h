#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>

class DXP_BackAcknowledge: public DXP_AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_BackAcknowledge(const std::string&);
        DXP_BackAcknowledge(AcceptanceCode);

        // views
        virtual DXP_String message(void) const;

        AcceptanceCode acceptance_code(void) const;

private:
        // implementation
        static const char HEADER = 'K';
        static const bool REGISTERED;

        // representation
        AcceptanceCode d_acceptance_code;
};
