#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>

class DXP_GameAcknowledge: public DXP_AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        DXP_GameAcknowledge(const std::string&, AcceptanceCode);

        // views
        virtual DXP_StringMessage message(void) const;

        const std::string& name_follower(void) const;
        AcceptanceCode acceptance_code(void) const;

private:    
        // implementation
        explicit DXP_GameAcknowledge(const std::string&);

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};
