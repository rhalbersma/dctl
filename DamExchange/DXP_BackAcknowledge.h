#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>

class DXP_BackAcknowledge: public DXP_AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        explicit DXP_BackAcknowledge(AcceptanceCode);

        // views
        virtual DXP_StringMessage message(void) const;

        AcceptanceCode acceptance_code(void) const;

private:
        // implementation
        explicit DXP_BackAcknowledge(const std::string&);

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        AcceptanceCode acceptance_code_;
};
